#include <iocv/COcvBlobProcessorComp.h>


// OpenCV includes
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/opencv.hpp>

// ACF includes
#include <istd/CChangeGroup.h>
#include <iimg/CBitmapBase.h>
#include <iimg/CScanlineMask.h>
#include <iprm/TParamsPtr.h>
#include <i2d/CPolygon.h>
#include <ilog/CExtMessage.h>
#include <iimg/CBitmap.h>

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

	// Initialize input bitmap:
	cv::Mat tmpBinaryImage;

	// Filter out masked points:
	if (aoiPtr != NULL){
		iimg::CScanlineMask mask;
		mask.SetCalibration(image.GetCalibration());

		i2d::CRect clipArea(size);
		if (!mask.CreateFromGeometry(*aoiPtr, &clipArea)){
			SendErrorMessage(0, QObject::tr("AOI type is not supported"));

			return false;
		}

		iimg::CBitmap maskedBitmap;
		maskedBitmap.CreateBitmap(image.GetPixelFormat(), image.GetImageSize(), image.GetPixelBitsCount(), image.GetComponentsCount());
		maskedBitmap.ClearImage();

		istd::CIntRange lineRange(0, size.GetX());

		int bytesPerPixel = image.GetPixelBitsCount() / 8;

		for (int y = 0; y < size.GetY(); ++y){
			const istd::CIntRanges* outputRangesPtr = mask.GetPixelRanges(y);
			if (outputRangesPtr != NULL){
				const quint8* inputLinePtr = (const quint8*)image.GetLinePtr(y);
				quint8* outputLinePtr = (quint8*)maskedBitmap.GetLinePtr(y);

				istd::CIntRanges::RangeList rangeList;
				outputRangesPtr->GetAsList(lineRange, rangeList);
				for (		istd::CIntRanges::RangeList::ConstIterator iter = rangeList.constBegin();
							iter != rangeList.constEnd();
							++iter){
					const istd::CIntRange& rangeH = *iter;
					Q_ASSERT(rangeH.GetMinValue() >= 0);
					Q_ASSERT(rangeH.GetMaxValue() <= size.GetX());

					int copyPixelsCount = rangeH.GetLength();

					memcpy(outputLinePtr + rangeH.GetMinValue(), inputLinePtr + rangeH.GetMinValue(), copyPixelsCount * bytesPerPixel);
				}
			}
		}

		void* imageDataBufferPtr = const_cast<void*>(maskedBitmap.GetLinePtr(0));
		cv::Mat inputBitmap(size.GetY(), size.GetX(), CV_8UC1, imageDataBufferPtr, maskedBitmap.GetLineBytesCount());
		tmpBinaryImage = inputBitmap.clone();
	}
	else{
		void* imageDataBufferPtr = const_cast<void*>(image.GetLinePtr(0));
		cv::Mat inputBitmap(size.GetY(), size.GetX(), CV_8UC1, imageDataBufferPtr, image.GetLineBytesCount());

		tmpBinaryImage = inputBitmap.clone();
	}

	// Get contours from the binary image:
	std::vector<std::vector<cv::Point> > contours;
	findContours(tmpBinaryImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	// Get found blobs:
	int blobsCount = 0;

	istd::TDelPtr<ilog::CExtMessage> blobMessagePtr;
	if (m_resultConsumerCompPtr.IsValid()){
		blobMessagePtr.SetPtr(new ilog::CExtMessage(
			istd::IInformationProvider::IC_INFO,
			MI_FOUND_BLOB,
			"",
			"OpenCV Blob Detector"));
	}

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

		if (passedByFilter){
			blobsCount++;

			i2d::CVector2d position = i2d::CVector2d(location.x, location.y);

			iblob::CBlobFeature* blobFeaturePtr = new iblob::CBlobFeature(area, perimeter, position);
			result.AddFeature(blobFeaturePtr);

			if (blobMessagePtr.IsValid()){
				i2d::CCircle* blobMessageCirclePtr = new imod::TModelWrap<i2d::CCircle>();
				blobMessageCirclePtr->SetPosition(position);
				blobMessageCirclePtr->SetRadius(qSqrt(area / I_PI));
				blobMessagePtr->InsertAttachedObject(blobMessageCirclePtr, QObject::tr("Blob %1, Pos.: (%2; %3), Area: %4").arg(blobsCount).arg(position.GetX()).arg(position.GetY()).arg(area));
			}
		}
	}

	if (blobMessagePtr.IsValid()){
		blobMessagePtr->SetText(QObject::tr("Found %1 blob(s)").arg(blobsCount));

		m_resultConsumerCompPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(blobMessagePtr.PopPtr()));
	}

	return true;
}


} // namespace iocv


