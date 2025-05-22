#include <iocv/CHoughBasedSearchProcessorComp.h>


// Qt includes
#include <QtCore/QElapsedTimer>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iprm/TParamsPtr.h>
#include <ifile/IFileNameParam.h>
#include <iimg/CBitmap.h>
#include <i2d/CRect.h>
#include <iipr/CSearchFeature.h>

// OpenCV includes
#include "opencv2/opencv.hpp"


namespace iocv
{


// public methods

// reimplemented (iipr::IImageToFeatureProcessor)

iproc::IProcessor::TaskState CHoughBasedSearchProcessorComp::DoExtractFeatures(
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

	int minThreshold = 50;
	int maxThreshold = 150;
	iprm::TParamsPtr<imeas::INumericValue> edgeThresholdParamPtr(paramsPtr, m_edgeThresholdParamIdAttrPtr, m_defaultEdgeThresholdCompPtr);
	if (edgeThresholdParamPtr.IsValid()){
		minThreshold = edgeThresholdParamPtr->GetValues()[0] * 255;

		if (edgeThresholdParamPtr->GetValues().GetElementsCount() > 1){
			maxThreshold = edgeThresholdParamPtr->GetValues()[1] * 255;
		}
	}

	int houghThreshold = 100;
	iprm::TParamsPtr<imeas::INumericValue> houghThresholdParamPtr(paramsPtr, m_houghThresholdParamIdAttrPtr, m_defaultHoughThresholdCompPtr);
	if (houghThresholdParamPtr.IsValid()){
		houghThreshold = houghThresholdParamPtr->GetValues()[0];
	}

	switch (*m_searchModelTypeAttrPtr){
		case MT_CIRCLE:{
			int minRadius = 3;
			int maxRadius = 100;
			iprm::TParamsPtr<imeas::INumericValue> radiusRangeParamPtr(paramsPtr, m_radiusRangeParamIdAttrPtr, m_defaultRadiusRangeCompPtr);
			if (radiusRangeParamPtr.IsValid()){
				minRadius = radiusRangeParamPtr->GetValues()[0];
				maxRadius = radiusRangeParamPtr->GetValues()[1];
			}

			int foundModelsCount = DoCircleSearch(image, aoiRect, scale, minDistance, minThreshold, houghThreshold, minRadius, maxRadius, results);
			if (foundModelsCount < 0){
				return TS_INVALID;
			}
			break;
		}

		case MT_LINE:{
			int foundModelsCount = DoLineSearch(image, aoiRect, scale, minDistance, minThreshold, houghThreshold, results);
			if (foundModelsCount < 0){
				return TS_INVALID;
			}
			break;
		}

		case MT_TEMPLATE:{
			istd::CRange scaleRange(0.8, 1.2);
			double scaleStep(0.05);
			istd::CRange angleRange(0, 10);
			double angleStep = 1;

			iprm::TParamsPtr<ifile::IFileNameParam> modelImagePathParamPtr(paramsPtr, *m_modelImagePathAttrPtr);
			if (modelImagePathParamPtr.IsValid() && !modelImagePathParamPtr->GetPath().isEmpty()){

				QImage loaded;
				if (loaded.load(modelImagePathParamPtr->GetPath())){
					iimg::CBitmap modelImage;
					modelImage.CopyImageFrom(loaded);

					int foundModelsCount = DoTemplateSearch(image, modelImage, aoiRect, scaleRange, scaleStep, angleRange, angleStep, minDistance, 360, istd::CRange(minThreshold, maxThreshold), results);
					if (foundModelsCount < 0){
						return TS_INVALID;
					}
				}
			}
			else{
				return TS_INVALID;
			}
			break;
		}

		default:
			return TS_INVALID;
	}

	return TS_OK;
}


// reimplemented (iproc::IProcessor)

iproc::IProcessor::TaskState CHoughBasedSearchProcessorComp::DoProcessing(
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
	int flags = SSF_SCALING | SSF_TRANSLATION | SSF_MULTIPLE_RESULTS;

	switch (*m_searchModelTypeAttrPtr){
		case MT_LINE:
		case MT_TEMPLATE:
			flags |= SSF_ROTATION;
			break;

		default:
			break;
	}

	return flags;
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

int CHoughBasedSearchProcessorComp::DoCircleSearch(
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

	i2d::CVector2d regionOffset(aoi.GetLeft(), aoi.GetTop());

	int foundModelsCount = 0;
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(cvImage, circles, cv::HOUGH_GRADIENT, scale, minDistance, edgeThreshold, houghThreshold, minRadius, maxRadius);

	for (size_t circleIndex = 0; circleIndex < circles.size(); circleIndex++){
		const cv::Vec3f& circle = circles[circleIndex];

		CircleFeature* circleFeaturePtr = new CircleFeature(1.0);

		circleFeaturePtr->SetPosition(regionOffset + i2d::CVector2d(circle[0], circle[1]));
		circleFeaturePtr->SetRadius(circle[2]);

		const i2d::ICalibration2d* bitmapCalibrationPtr = image.GetCalibration();
		if (bitmapCalibrationPtr != NULL){
			circleFeaturePtr->Transform(*bitmapCalibrationPtr);
		}

		result.AddFeature(circleFeaturePtr);
		foundModelsCount++;
	}

	if (IsVerboseEnabled()){
		SendVerboseMessage(QT_TR_NOOP(QString("Circle search took %1 ms").arg(timer.elapsed())));
	}

	return foundModelsCount;
}



int CHoughBasedSearchProcessorComp::DoLineSearch(
			const iimg::IBitmap& /*image*/,
			const i2d::CRect& /*aoi*/,
			int /*scale*/,
			int /*minDistance*/,
			int /*edgeThreshold*/,
			int /*houghThreshold*/,
			iipr::IFeaturesConsumer& /*result*/)
{
	return -1;
}


int CHoughBasedSearchProcessorComp::DoTemplateSearch(
			const iimg::IBitmap& image,
			const iimg::IBitmap& modelImage,
			const i2d::CRect& aoi,
			const istd::CRange& scaleRange,
			double scaleStep,
			const istd::CRange& angleRange,
			double angleStep,
			int minDistance,
			int levels,
			const istd::CRange& cannyEdgeThreshold,
			iipr::IFeaturesConsumer& result)
{
	const int angleThresh = 100;
	const int scaleThresh = 100;
	const int posThresh = 100;
	const double dp = 1;
	const double minScale = scaleRange.GetMinValue();
	const double maxScale = scaleRange.GetMaxValue();
	const double minAngle = angleRange.GetMinValue();
	const double maxAngle = angleRange.GetMaxValue();
	const int maxBufSize = 1000;

	cv::Ptr<cv::GeneralizedHoughGuil> guil = cv::createGeneralizedHoughGuil();

	cv::Mat inputImage(aoi.GetHeight(), aoi.GetWidth(), CV_8UC1, (quint8*)image.GetLinePtr(aoi.GetTop()) + aoi.GetLeft(), image.GetLineBytesCount());
	cv::Mat templateImage(modelImage.GetImageSize().GetY(), modelImage.GetImageSize().GetX(), CV_8UC1, (quint8*)modelImage.GetLinePtr(0), modelImage.GetLineBytesCount());

	guil->setMinDist(minDistance);
	guil->setLevels(levels);
	guil->setDp(dp);
	guil->setMaxBufferSize(maxBufSize);

	guil->setMinAngle(minAngle);
	guil->setMaxAngle(maxAngle);
	guil->setAngleStep(angleStep);
	guil->setAngleThresh(angleThresh);

	guil->setMinScale(minScale);
	guil->setMaxScale(maxScale);
	guil->setScaleStep(scaleStep);
	guil->setScaleThresh(scaleThresh);

	guil->setPosThresh(posThresh);

	guil->setCannyLowThresh(cannyEdgeThreshold.GetMinValue());
	guil->setCannyHighThresh(cannyEdgeThreshold.GetMaxValue());

	std::vector<cv::Vec4f> position;

	guil->setTemplate(templateImage);
	guil->detect(inputImage, position);

	for (size_t i = 0; i < position.size(); ++i){
		i2d::CVector2d modelPosition(position[i][0], position[i][1]);
		float scale = position[i][2];
		float angle = position[i][3];

		iipr::CSearchFeature* searchFeaturePtr = new iipr::CSearchFeature(1.0, modelPosition, angle, i2d::CVector2d(scale, scale));

		result.AddFeature(searchFeaturePtr);
	}

	return int(position.size());
}


} // namespace iocv


