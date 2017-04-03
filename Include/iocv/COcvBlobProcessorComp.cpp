#include <iocv/COcvBlobProcessorComp.h>


// OpenCV includes
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>

// ACF includes
#include <istd/CChangeGroup.h>
#include <iimg/CBitmapBase.h>
#include <iimg/CScanlineMask.h>
#include <iprm/TParamsPtr.h>
#include <i2d/CPolygon.h>

// ACF-Solutions includes
#include <iblob/CBlobFeature.h>


namespace iocv
{


// protected methods

// reimplemented (iblob::CBlobProcessorCompBase)

bool COcvBlobProcessorComp::CalculateBlobs(
			const iprm::IParamsSet* /*paramsPtr*/,
			const iblob::IBlobFilterParams* filterParamsPtr,
			const i2d::IObject2d* aoiPtr,
			const iimg::IBitmap& image,
			iipr::IFeaturesConsumer& result)
{
	istd::CChangeGroup resultNotifier(&result);

	result.ResetFeatures();

	ibase::CSize size = image.GetImageSize();

	// create image mask
	i2d::CRect clipArea(size);
	iimg::CScanlineMask mask;
	if (aoiPtr != NULL){
		mask.SetCalibration(image.GetCalibration());

		if (!mask.CreateFromGeometry(*aoiPtr, &clipArea)){
			SendErrorMessage(0, QObject::tr("AOI type is not supported"));

			return false;
		}
	}
	else{
		mask.CreateFilled(clipArea);
	}

	// Initialize input bitmap:
	void* imageDataBufferPtr = const_cast<void*>(image.GetLinePtr(0));
	cv::Mat inputBitmap(image.GetImageSize().GetY(), image.GetImageSize().GetX(), CV_8UC1, imageDataBufferPtr, image .GetLineBytesCount());

	// Get contours from the binary image:
	cv::Mat tmpBinaryImage = inputBitmap.clone();
	std::vector<std::vector<cv::Point> > contours;
	findContours(tmpBinaryImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	// Get found blobs:
	for (int contourIndex = 0; contourIndex < int(contours.size()); contourIndex++){
		cv::Moments moms = cv::moments(cv::Mat(contours[contourIndex]));

		double area = moms.m00;
		if (qFuzzyCompare(area, 0.0)){
			continue;
		}

		double perimeter = cv::arcLength(cv::Mat(contours[contourIndex]), true);

		cv::Point2d location = cv::Point2d(moms.m10 / moms.m00, moms.m01 / moms.m00);
		double circularity = 0.0;
		
		if (perimeter > 0){
			circularity = 4 * I_PI * area / (perimeter * perimeter);
		}

		bool passedByFilter = true;

		if (filterParamsPtr != NULL){
			passedByFilter = IsBlobAcceptedByFilter(*filterParamsPtr, area, perimeter, circularity);
		}

		bool passedByMask = true;

		if (aoiPtr != NULL){
			passedByMask = aoiPtr->GetBoundingBox().Contains(istd::CIndex2d(location.x, location.y));
		}

		if (passedByFilter && passedByMask){
			iblob::CBlobFeature* blobFeaturePtr = new iblob::CBlobFeature(area, perimeter, i2d::CVector2d(location.x, location.y));

			result.AddFeature(blobFeaturePtr);
		}
	}

	return true;
}


} // namespace iocv


