#include <iocv/CHoughBasedSearchProcessorComp.h>


// Qt includes
#include <QtCore/QElapsedTimer>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iprm/TParamsPtr.h>
#include <i2d/CRect.h>

// OpenCV includes
#include <opencv/cv.h>
#include <opencv/highgui.h>


namespace iocv
{


// public methods

// reimplemented (iipr::IImageToFeatureProcessor)

int CHoughBasedSearchProcessorComp::DoExtractFeatures(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap& image,
			iipr::IFeaturesConsumer& results,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
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

		aoiRect = i2d::CRect(int(bbox.GetLeft()), int(bbox.GetTop()), int(bbox.GetRight()), int(bbox.GetBottom()));
	}

	int scale = 1;
	iprm::TParamsPtr<imeas::INumericValue> scaleParamPtr(paramsPtr, m_scaleParamIdAttrPtr, m_defaultScaleCompPtr);
	if (scaleParamPtr.IsValid()){
		scale = scaleParamPtr->GetValues()[0];
	}

	int minDistance = 50;
	iprm::TParamsPtr<imeas::INumericValue> minDistanceParamPtr(paramsPtr, m_minDistanceParamIdAttrPtr, m_defaultMinDistanceCompPtr);
	if (minDistanceParamPtr.IsValid()){
		minDistance = minDistanceParamPtr->GetValues()[0];
	}

	int edgeThreshold = 100;
	iprm::TParamsPtr<imeas::INumericValue> edgeThresholdParamPtr(paramsPtr, m_edgeThresholdParamIdAttrPtr, m_defaultEdgeThresholdCompPtr);
	if (edgeThresholdParamPtr.IsValid()){
		edgeThreshold = edgeThresholdParamPtr->GetValues()[0];
	}

	int houghThreshold = 100;
	iprm::TParamsPtr<imeas::INumericValue> houghThresholdParamPtr(paramsPtr, m_houghThresholdParamIdAttrPtr, m_defaultHoughThresholdCompPtr);
	if (houghThresholdParamPtr.IsValid()){
		houghThreshold = houghThresholdParamPtr->GetValues()[0];
	}

	int minRadius = 0;
	int maxRadius = 0;
	iprm::TParamsPtr<imeas::INumericValue> radiusRangeParamPtr(paramsPtr, m_radiusRangeParamIdAttrPtr, m_defaultRadiusRangeCompPtr);
	if (radiusRangeParamPtr.IsValid()){
		minRadius = radiusRangeParamPtr->GetValues()[0];
		maxRadius = radiusRangeParamPtr->GetValues()[1];
	}

	int foundModelsCount = DoModelSearch(image, aoiRect, scale, minDistance, edgeThreshold, houghThreshold, minRadius, maxRadius, results);
	if (foundModelsCount < 0){
		// search was unsuccessful
		return TS_INVALID;
	}

	return TS_OK;
}


// reimplemented (iproc::IProcessor)

int CHoughBasedSearchProcessorComp::DoProcessing(
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

int CHoughBasedSearchProcessorComp::GetSearchSupportedFlags() const
{
	return SSF_TRANSLATION | SSF_MULTIPLE_RESULTS;
}


const istd::CRange& CHoughBasedSearchProcessorComp::GetRotationRangeConstraints() const
{
	static istd::CRange range(0, 0);

	return range;
}


const istd::CRange& CHoughBasedSearchProcessorComp::GetScaleRangeConstraints() const
{
	static istd::CRange range(0, 0);

	return range;
}


const istd::CIntRange& CHoughBasedSearchProcessorComp::GetResultsCountConstraints() const
{
	static istd::CIntRange range(0, 0);

	return range;
}


// protected methods

int CHoughBasedSearchProcessorComp::DoModelSearch(
			const iimg::IBitmap& image,
			const i2d::CRect& aoi,
			int scale,
			int minDistance,
			int edgeThreshold,
			int houghThreshold,
			int minRadius,
			int maxRadius,
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

	istd::CChangeNotifier resultNotifier(&result);

	result.ResetFeatures();

	cv::Mat cvImage(aoi.GetHeight(), aoi.GetWidth(), CV_8UC1, (quint8*)image.GetLinePtr(aoi.GetTop()) + aoi.GetLeft(), image.GetLineBytesCount());
	IplImage inputImage(cvImage);

	i2d::CVector2d regionOffset(aoi.GetLeft(), aoi.GetTop());

	int foundModelsCount = 0;

	switch (*m_searchModelTypeAttrPtr){
		case MT_CIRCLE:{
			CvMemStorage* storagePtr = cvCreateMemStorage(0);
			CvSeq* circles = cvHoughCircles(&inputImage, storagePtr, CV_HOUGH_GRADIENT, scale, minDistance, edgeThreshold, houghThreshold, minRadius, maxRadius);

			foundModelsCount = circles->total;
			for (int circleIndex = 0; circleIndex < circles->total; circleIndex++){
				cv::Vec3f circle = *((cv::Vec3f*)cvGetSeqElem(circles, circleIndex));

				CircleFeature* circleFeaturePtr = new CircleFeature(1.0);

				circleFeaturePtr->SetPosition(regionOffset + i2d::CVector2d(circle[0], circle[1]));
				circleFeaturePtr->SetRadius(circle[2]);

				result.AddFeature(circleFeaturePtr);
			}

			cvReleaseMemStorage(&storagePtr);
			break;
		}

		default:
			break;
	}

	if (IsVerboseEnabled()){
		SendVerboseMessage(QObject::tr("Model search took %1 ms").arg(timer.elapsed()));
	}

	return foundModelsCount;
}


} // namespace iocv


