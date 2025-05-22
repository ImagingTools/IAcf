#include <iocv/COcvBlobProcessorComp.h>


// ACF includes
#include <istd/CChangeGroup.h>
#include <iimg/CBitmapBase.h>
#include <iimg/CScanlineMask.h>
#include <iprm/TParamsPtr.h>
#include <i2d/CPolygon.h>
#include <ilog/CExtMessage.h>
#include <iimg/CBitmap.h>
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
			SendErrorMessage(0, QString("AOI type is not supported"));
			return false;
		}

		if (mask.IsEmpty()) {
			SendErrorMessage(0, QString("No intersection of AOI with image"));
			return false;
		}

		iimg::CBitmap maskedBitmap;
		maskedBitmap.CreateBitmap(image.GetPixelFormat(), image.GetImageSize(), image.GetPixelBitsCount(), image.GetComponentsCount());
		maskedBitmap.ClearImage();

		istd::CIntRange lineRange(0, size.GetX());

		int bytesPerPixel = image.GetPixelBitsCount() >> 3;

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
		cv::Mat inputBitmap(size.GetY(), size.GetX(), CV_8UC1, imageDataBufferPtr, maskedBitmap.GetLinesDifference());
		tmpBinaryImage = inputBitmap.clone();
	}
	else{
		void* imageDataBufferPtr = const_cast<void*>(image.GetLinePtr(0));
		cv::Mat inputBitmap(size.GetY(), size.GetX(), CV_8UC1, imageDataBufferPtr, image.GetLinesDifference());

		tmpBinaryImage = inputBitmap.clone();
	}

	// Get contours from the binary image:
	std::vector<std::vector<cv::Point2f>> contours;
	{
		std::vector<std::vector<cv::Point>> contours_int; // integer positions
		auto contourMode = CV_RETR_EXTERNAL;
		if (m_contourRetrievalModeAttrPtr.IsValid()){
			if (*m_contourRetrievalModeAttrPtr == (int)(CRT_LIST))
				contourMode = CV_RETR_LIST;
			else if (*m_contourRetrievalModeAttrPtr == (int)(CRT_TREE))
				contourMode = CV_RETR_TREE;
			//otherwise default = CV_RETR_EXTERNAL
		}
		cv::findContours(tmpBinaryImage, contours_int, contourMode, CV_CHAIN_APPROX_SIMPLE);

		// ocv seemingly defines the contours on the top and left borders of "white" pixels, thus the positions proove to be shifted by max. one pixel
		// shifting all positions by (0.5, 0.5) makes the systematic error uniform, its value being less or equal to 0.5 pixels
		contours.reserve(contours.size());
		std::transform(contours_int.begin(), contours_int.end(), std::back_inserter(contours),
					   [](const std::vector<cv::Point>& item) {
						   std::vector<cv::Point2f> result;
						   result.reserve(item.size());

						   std::transform(item.begin(), item.end(), std::back_inserter(result),
										  [](const cv::Point& p) { return cv::Point2f(p.x + 0.5f, p.y + 0.5f); });
						   return result;
					   }
		);
	}
	

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

	const i2d::ICalibration2d* calibrationPtr = m_calibrationProviderCompPtr.IsValid() ? m_calibrationProviderCompPtr->GetCalibration() : NULL;

	for (int contourIndex = 0; contourIndex < int(contours.size()); contourIndex++){
		cv::Mat points(contours[contourIndex]);
		cv::Moments moms = cv::moments(points);

		const double orientedArea = cv::contourArea(points, true);	//isnt equal moms.m00 ?
		const double areaPixels = qAbs(orientedArea);
		if (qFuzzyCompare(areaPixels, 0.0)){
			continue;
		}

		cv::RotatedRect rotatedRect = cv::minAreaRect(points);
		float angle = qDegreesToRadians(rotatedRect.angle);

		cv::Point2d location = cv::Point2d(moms.m10 / moms.m00, moms.m01 / moms.m00);

		double circularity = 0.0;
		double perimeterPixels = cv::arcLength(points, true);
		if (perimeterPixels > 0){
			circularity = 4 * I_PI * areaPixels / (perimeterPixels * perimeterPixels);
		}

		bool passedByFilter = true;

		if (filterParamsPtr != NULL){
			// convert pix to mm to match metric filter
			if (calibrationPtr) {
				i2d::CPolygon polygonMM;
				for (const cv::Point2f& p : contours[contourIndex]) {
					i2d::CVector2d v(p.x, p.y);
					i2d::CVector2d mm;
					if (calibrationPtr->GetInvPositionAt(v, mm)) {
						v = mm;
					}
					polygonMM.InsertNode(v);
				}

				double areaMM = polygonMM.GetArea();
				double perimeterMM = polygonMM.GetPerimeter();

				passedByFilter = IsBlobAcceptedByFilter(*filterParamsPtr, areaMM, perimeterMM, circularity);
			}
			else {
				passedByFilter = IsBlobAcceptedByFilter(*filterParamsPtr, areaPixels, perimeterPixels, circularity);
			}
		}

		if (m_getNegativeBlobsPolygonCompPtr.IsValid()){
			const bool isNegative = m_getNegativeBlobsPolygonCompPtr->IsEnabled();

			passedByFilter = passedByFilter && isNegative == (orientedArea > 0);
		}

		if (passedByFilter){
			blobsCount++;

			i2d::CVector2d position(location.x, location.y);
			if (calibrationPtr != nullptr){
				i2d::CVector2d positionMM;
				if (calibrationPtr->GetInvPositionAt(position, positionMM)){
					position = positionMM;
				}
			}

			i2d::CPolygon polygon;
			polygon.SetCalibration(calibrationPtr);

			for (const cv::Point2f& p : contours[contourIndex]){
				i2d::CVector2d v(p.x, p.y);

				if (calibrationPtr != NULL){
					i2d::CVector2d mm;
					if (calibrationPtr->GetInvPositionAt(v, mm)){
						v = mm;
					}
				}

				polygon.InsertNode(v);
			}

			iblob::CBlobFeature* blobFeaturePtr = new iblob::CBlobFeature(areaPixels, perimeterPixels, position, angle, i2d::CVector2d(1, 1), polygon);
			result.AddFeature(blobFeaturePtr);

			blobFeaturePtr->SetObjectId(QByteArray::number(contourIndex));

			// weight is diameter
			double d = (polygon.GetBoundingBox().GetWidth() + polygon.GetBoundingBox().GetHeight()) / 2;
			blobFeaturePtr->SetWeight(d);

			if (blobMessagePtr.IsValid()){
				QString px2 = QObject::tr("px") + QCHAR_POW2;
				QString mm2 = QObject::tr("mm") + QCHAR_POW2;

				double metricArea = blobFeaturePtr->GetArea();
				i2d::CPolygon* blobMessagePolygonPtr = new imod::TModelWrap<i2d::CPolygon>();
				blobMessagePolygonPtr->CopyFrom(polygon, istd::IChangeable::CM_CONVERT);
				blobMessagePtr->InsertAttachedObject(
							blobMessagePolygonPtr,
							QObject::tr("Blob %1, Position: (%2, %3), Area: %4 %5 (%6 %7)")
										.arg(blobsCount)
										.arg(position.GetX()).arg(position.GetY())
										.arg(areaPixels)
										.arg(px2)
										.arg(metricArea)
										.arg(mm2)
				);
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


