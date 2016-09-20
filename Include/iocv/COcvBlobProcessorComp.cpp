#include <iocv/COcvBlobProcessorComp.h>


// OpenCV includes
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iimg/CBitmapBase.h>
#include <iprm/TParamsPtr.h>
#include <i2d/CPolygon.h>

// ACF-Solutions includes
#include <iblob/CBlobFeature.h>


namespace iocv
{


// public methods

// reimplemented (iipr::IImageToFeatureProcessor)

int COcvBlobProcessorComp::DoExtractFeatures(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap& image,
			iipr::IFeaturesConsumer& results,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	if (paramsPtr == NULL){
		return TS_INVALID;
	}

	iprm::TParamsPtr<iblob::IBlobFilterParams> filterParamsPtr(paramsPtr, *m_filterParamsIdAttrPtr);

	bool retVal = CalculateBlobs(image, filterParamsPtr.GetPtr(), results)? TS_OK: TS_INVALID;
	if (!retVal){
		return TS_INVALID;
	}

	return retVal ? TS_OK: TS_INVALID;
}


// reimplemented (iproc::IProcessor)

int COcvBlobProcessorComp::DoProcessing(
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


// protected methods

bool COcvBlobProcessorComp::CalculateBlobs(
			const iimg::IBitmap& bitmap,
			const iblob::IBlobFilterParams* blobFilterParamsPtr,
			iipr::IFeaturesConsumer& result)
{
	istd::CChangeNotifier resultNotifier(&result);

	result.ResetFeatures();

	// Initialize input bitmap:
	void* imageDataBufferPtr = const_cast<void*>(bitmap.GetLinePtr(0));
	cv::Mat inputBitmap(bitmap.GetImageSize().GetY(), bitmap.GetImageSize().GetX(), CV_8UC1, imageDataBufferPtr, bitmap.GetLineBytesCount());

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

		if ((blobFilterParamsPtr != NULL) && blobFilterParamsPtr->IsFiltersEnabled()){
			int filtersCount = blobFilterParamsPtr->GetFiltersCount();

			for (int filterIndex = 0; filterIndex < filtersCount; ++filterIndex){
				iblob::IBlobFilterParams::Filter filter = blobFilterParamsPtr->GetFilterAt(filterIndex);

				if (filter.blobDescriptorType == iblob::CBlobDescriptorInfo::BDT_CIRCULARITY){
					passedByFilter = passedByFilter && IsValueAcceptedByFilter(filter, circularity);
				}

				if (filter.blobDescriptorType == iblob::CBlobDescriptorInfo::BDT_AREA){
					passedByFilter = passedByFilter && IsValueAcceptedByFilter(filter, area);
				}

				if (filter.blobDescriptorType == iblob::CBlobDescriptorInfo::BDT_PERIMETER){
					passedByFilter = passedByFilter && IsValueAcceptedByFilter(filter, perimeter);
				}
			}
		}

		if (passedByFilter){
			iblob::CBlobFeature* blobFeaturePtr = new iblob::CBlobFeature(area, perimeter, i2d::CVector2d(location.x, location.y));

			result.AddFeature(blobFeaturePtr);
		}
	}

	return true;
}


// private static methods

bool COcvBlobProcessorComp::IsValueAcceptedByFilter(const iblob::IBlobFilterParams::Filter& filter, double value)
{
	bool isGreater = value > filter.valueRange.GetMinValue();
	bool isLess = value > filter.valueRange.GetMinValue();
	bool isEqual = qFuzzyCompare(value, filter.valueRange.GetMinValue());
	bool isGreaterEqual = isGreater || isEqual;
	bool isLessEqual = isLess || isEqual;

	switch(filter.condition)
	{
	case iblob::IBlobFilterParams::FC_EQUAL:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? isEqual : !isEqual;
	case iblob::IBlobFilterParams::FC_NOT_EQUAL:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? !isEqual : isEqual;
	case iblob::IBlobFilterParams::FC_BETWEEN:
	{
		bool valueInRange = filter.valueRange.Contains(value);
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? valueInRange : !valueInRange;
	}
	case iblob::IBlobFilterParams::FC_OUTSIDE:
	{
		bool valueInRange = filter.valueRange.Contains(value);
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? !valueInRange : valueInRange;
	}
	case iblob::IBlobFilterParams::FC_GREATER:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? isGreater : !isGreater;
	case iblob::IBlobFilterParams::FC_GREATER_EQUAL:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? isGreaterEqual : !isGreaterEqual;
	case iblob::IBlobFilterParams::FC_LESS:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? isLess : !isLess;
	case iblob::IBlobFilterParams::FC_LESS_EQUAL:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? isLessEqual : !isLessEqual;
	}

	return true;
}


} // namespace iocv


