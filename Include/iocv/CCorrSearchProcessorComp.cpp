#include <iocv/CCorrSearchProcessorComp.h>


// OpenCV includes
#include "opencv2/opencv.hpp"
#include <opencv/highgui.h>

// Qt includes
#include <QtCore/QElapsedTimer>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iprm/TParamsPtr.h>
#include <i2d/CRect.h>
#include <iipr/CSearchFeature.h>


namespace iocv
{


// public methods

// reimplemented (iipr::IImageToFeatureProcessor)

int CCorrSearchProcessorComp::DoExtractFeatures(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap& image,
			iipr::IFeaturesConsumer& results,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	iprm::TParamsPtr<iipr::ISearchParams> searchParamsPtr(paramsPtr, m_searchParamsIdAttrPtr, m_defaultSearchParamsCompPtr, false);
	iprm::TParamsPtr<iimg::IBitmap> modelImagePtr(paramsPtr, m_modelParamIdAttrPtr, m_defaultModelImageCompPtr, true);

	if (!modelImagePtr.IsValid()){
		return false;
	}

	istd::TDelPtr<i2d::IObject2d> usedAoiPtr;

	istd::CIndex2d imageSize = image.GetImageSize();
	i2d::CRect aoiRect = i2d::CRect(0, 0, imageSize.GetX(), imageSize.GetY());

	iprm::TParamsPtr<i2d::IObject2d> aoiPtr(paramsPtr, m_aoiParamIdAttrPtr, m_defaultAoiCompPtr, false);
	if (aoiPtr.IsValid()){
		// Clone calibration to calculate image aoi in image coordinates
		usedAoiPtr.SetCastedOrRemove(aoiPtr->CloneMe(istd::IChangeable::CM_WITHOUT_REFS));
		if (!usedAoiPtr.IsValid()){
			SendErrorMessage(0, "Recalibration of the region failed");

			return TS_INVALID;
		}

		// Reset possible existing calibration:
		usedAoiPtr->SetCalibration(image.GetCalibration(), false);

		// Do convert copy from the source object:
		if (!usedAoiPtr->CopyFrom(*aoiPtr.GetPtr(), istd::IChangeable::CM_CONVERT)){
			SendErrorMessage(0, "Recalibration of the region failed");

			return TS_INVALID;
		}

		i2d::CRectangle bbox = aoiPtr->GetBoundingBox();

		aoiRect = i2d::CRect(int(bbox.GetLeft()), int(bbox.GetRight()), int(bbox.GetTop()), int(bbox.GetBottom()));
	}

	double minScore = 0.7;
	if (searchParamsPtr.IsValid()){
		minScore = searchParamsPtr->GetMinScore();
	}

	int foundModelsCount = DoModelSearch(image, *modelImagePtr, aoiRect, minScore, results);

	if (foundModelsCount < 0){
		// search was unsuccessful
		return TS_INVALID;
	}

	return TS_OK;
}


// reimplemented (iproc::IProcessor)

int CCorrSearchProcessorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* progressManagerPtr)
{
	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		return TS_INVALID;
	}

	if (outputPtr == NULL){
		return TS_OK;
	}

	iipr::IFeaturesConsumer* outputConsumerPtr = dynamic_cast<iipr::IFeaturesConsumer*>(outputPtr);
	if (outputConsumerPtr == NULL){
		return TS_INVALID;
	}

	return DoExtractFeatures(paramsPtr, *inputBitmapPtr, *outputConsumerPtr, progressManagerPtr);
}


// reimplemented (iipr::ISearchConstraints)

int CCorrSearchProcessorComp::GetSearchSupportedFlags() const
{
	return SSF_TRANSLATION | SSF_SCORE_THRESHOLD;
}


const istd::CRange& CCorrSearchProcessorComp::GetRotationRangeConstraints() const
{
	static istd::CRange range(0, 0);

	return range;
}


const istd::CRange& CCorrSearchProcessorComp::GetScaleRangeConstraints() const
{
	static istd::CRange range(1, 1);

	return range;
}


const istd::CIntRange& CCorrSearchProcessorComp::GetResultsCountConstraints() const
{
	static istd::CIntRange range(1, 1);

	return range;
}


// protected methods

int CCorrSearchProcessorComp::DoModelSearch(
			const iimg::IBitmap& image,
			const iimg::IBitmap& modelImage,
			const i2d::CRect& aoi,
			double minScore,
			iipr::IFeaturesConsumer& result)
{
	istd::CIndex2d imageSize = image.GetImageSize();

	i2d::CRect searchRegion = aoi.GetIntersection(i2d::CRect(0, 0, imageSize.GetX(), imageSize.GetY()));
	if (searchRegion.IsEmpty()){
		SendErrorMessage(MI_BAD_PARAMS, "Search region is empty");

		return -1;
	}

	if (!searchRegion.IsValid()){
		SendErrorMessage(MI_BAD_PARAMS, "Search region is invalid");

		return -1;
	}

	QElapsedTimer timer;

	cv::Mat cvImage(aoi.GetHeight(), aoi.GetWidth(), CV_8UC1, (quint8*)image.GetLinePtr(aoi.GetTop()) + aoi.GetLeft(), image.GetLineBytesCount());

	istd::CIndex2d modelSize = modelImage.GetImageSize();
	cv::Mat cvModel(modelSize.GetX(), modelSize.GetY(), CV_8UC1, (quint8*)modelImage.GetLinePtr(0), modelImage.GetLineBytesCount());

	cv::Mat correlationMap;
	cv::matchTemplate(cvImage, cvModel, correlationMap, CV_TM_CCOEFF_NORMED);

	/// Localizing the best match with minMaxLoc
	double minVal;
	double maxVal;
	cv::Point minLoc;
	cv::Point maxLoc;
	cv::Point matchLoc;

	cv::minMaxLoc(correlationMap, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

	istd::CChangeNotifier resultNotifier(&result);

	result.ResetFeatures();

	i2d::CVector2d regionOffset(aoi.GetLeft(), aoi.GetTop());

	if (maxVal >= minScore){
		i2d::CVector2d position(maxLoc.x + aoi.GetLeft() + 0.5, maxLoc.y + aoi.GetTop() + 0.5);

		// return results
		result.AddFeature(new iipr::CSearchFeature(maxVal, regionOffset + position, 0, i2d::CVector2d(1, 1)));

		SendInfoMessage(0, QObject::tr("Found model, %1% score").arg(maxVal * 100));

		return 1;
	}

	if (IsVerboseEnabled()){
		SendVerboseMessage(QObject::tr("Model search took %1 ms").arg(timer.elapsed()));
	}

	return 0;
}


} // namespace iocv


