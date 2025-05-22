#include <iocv/COcvProcessorCompBase.h>


 // ACF includes
#include <istd/CIndex2d.h>
#include <ibase/CSize.h>
#include <iprm/TParamsPtr.h>
#include <iimg/IBitmap.h>
#include <iprm/IEnableableParam.h>
#include <imeas/INumericValue.h>
#include <iimg/CScanlineMask.h>
#include <i2d/IObject2d.h>

// OpenCV includes
#include <opencv2/imgproc/imgproc.hpp>


namespace iocv
{


// helper

static i2d::CPolygon GetCalibratedPolygon(const i2d::IObject2d& aoi)
{
	i2d::CPolygon polygon;

	// convert aoi to polygon if possible
	if (auto polyPtr = dynamic_cast<const i2d::CPolygon*>(&aoi)) {
		polygon = *polyPtr;
	}
	else if (auto cirlcePtr = dynamic_cast<const i2d::CCircle*>(&aoi)) {
		cirlcePtr->ConvertToPolygon(polygon);
	}
	else if (auto ringPtr = dynamic_cast<const i2d::CAnnulus*>(&aoi)) {
		ringPtr->ConvertToPolygon(polygon);
	}
	else
		return {};	// empty

	// decalibrate the polygon
	if (const i2d::ICalibration2d* logToPhysicalTransformPtr = aoi.GetCalibration()) {
		polygon.Transform(*logToPhysicalTransformPtr);
	}

	return polygon;
}


// protected methods

bool COcvProcessorCompBase::GetBitmaps(const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			const iimg::IBitmap*& inputBitmapPtr,
			iimg::IBitmap*& outputBitmapPtr) const
{
	inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL) {
		SendWarningMessage(0, "Input bitmap is not set");
		return false;
	}

	if (inputBitmapPtr->IsEmpty()) {
		SendWarningMessage(0, "Input bitmap is empty");
		return false;
	}

	outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (outputBitmapPtr == NULL) {
		SendWarningMessage(0, "Output bitmap is not set");
		return false;
	}

	if (!outputBitmapPtr->CopyFrom(*inputBitmapPtr)) {
		SendErrorMessage(0, "Data could not be copied from input bitmap to the output");
		return false;
	}

	// we are set
	return true;
}


void COcvProcessorCompBase::MakeProcessedRegionMatrices(const iimg::IBitmap* inputBitmapPtr,
			const iimg::IBitmap* outputBitmapPtr,
			const iprm::IParamsSet* paramsPtr,
			cv::Mat& inputMatrix,
			cv::Mat& outputMatrix) const
{
	if (m_aoiIdAttrPtr.IsValid()) {
		iprm::TParamsPtr<i2d::IObject2d> aoiPtr(paramsPtr, *m_aoiIdAttrPtr);
		if (aoiPtr.IsValid()) {
			auto aoiPixel = GetCalibratedPolygon(*aoiPtr);
			i2d::CRect bBox = aoiPixel.GetBoundingBox().GetIntersection(inputBitmapPtr->GetBoundingBox());

			const int pixelBits = inputBitmapPtr->GetPixelBitsCount();
			const int pixelBytes = pixelBits / 8;
			if (pixelBytes * 8 != pixelBits) {
				return;
			}

			quint8* startPtr = (quint8*)(inputBitmapPtr->GetLinePtr(bBox.GetTop())) + (bBox.GetLeft() * pixelBytes);
			inputMatrix = cv::Mat(bBox.GetHeight(), bBox.GetWidth(), CV_8UC1, startPtr, inputBitmapPtr->GetLinesDifference());
			outputMatrix = cv::Mat(inputMatrix.rows, inputMatrix.cols, CV_8UC1);
			return;
		}
	}
	
	// default entire 
	inputMatrix = cv::Mat(inputBitmapPtr->GetImageSize().GetY(), inputBitmapPtr->GetImageSize().GetX(), CV_8UC1, (quint8*)inputBitmapPtr->GetLinePtr(0));
	outputMatrix = cv::Mat(outputBitmapPtr->GetImageSize().GetY(), outputBitmapPtr->GetImageSize().GetX(), CV_8UC1, (quint8*)outputBitmapPtr->GetLinePtr(0));
}


bool COcvProcessorCompBase::EmplaceProcessedRegionBack(iimg::IBitmap* outputBitmapPtr,
			const iprm::IParamsSet* paramsPtr,
			cv::Mat& outputMatrix) const
{
	// AM: why do we check for this?
	int pixelBits = outputBitmapPtr->GetPixelBitsCount();
	int pixelBytes = pixelBits / 8;
	//qDebug() << "Output Pixel " << pixelBits << " bits";
	if (pixelBytes * 8 != pixelBits) {
		return false;
	}

	if (m_aoiIdAttrPtr.IsValid()) {
		iprm::TParamsPtr<i2d::IObject2d> aoiPtr(paramsPtr, *m_aoiIdAttrPtr);

		if (aoiPtr.IsValid()) {
			auto aoiPixel = GetCalibratedPolygon(*aoiPtr);
			aoiPixel.SetCalibration(nullptr);

			i2d::CRectangle imageBoundingBox = outputBitmapPtr->GetBoundingBox();
			i2d::CRect imageRect = imageBoundingBox;

			iimg::CScanlineMask regionMask;
			if (!regionMask.CreateFromGeometry(aoiPixel, &imageRect)) {
				SendErrorMessage(0, "Could not create Scanline Mask from Aoi geometry");
				return false;
			}

			i2d::CRect regionRect = aoiPixel.GetBoundingBox().GetIntersection(imageBoundingBox);
			int regionLeft = regionRect.GetLeft();
			//int regionRight = regionRect.GetRight();
			int regionTop = regionRect.GetTop();
			int regionBottom = regionRect.GetBottom();

			for (int y = regionTop; y < regionBottom; y++) {
				const istd::CIntRanges* rangesPtr = regionMask.GetPixelRanges(y);
				if (rangesPtr == NULL) {
					continue;
				}

				istd::CIntRanges::RangeList rangesList;
				rangesPtr->GetAsList(imageRect.GetHorizontalRange(), rangesList);

				quint8* originalLinePtr = (quint8*)outputBitmapPtr->GetLinePtr(y);
				cv::Mat matrixRow = outputMatrix.row(y - regionTop);

				for (istd::CIntRanges::RangeList::ConstIterator iter = rangesList.begin();
					iter != rangesList.end();
					++iter)
				{
					const istd::CIntRange& pixelRange = *iter;

					int rangeStart = qMax(pixelRange.GetMinValue(), 0);
					Q_ASSERT(rangeStart >= regionLeft);
					int rangeEnd = qMin(pixelRange.GetMaxValue(), outputBitmapPtr->GetImageSize().GetX());

					int bytesToCopy = (rangeEnd - rangeStart) * pixelBytes;

					if (bytesToCopy > 0) {
						std::memcpy(originalLinePtr + rangeStart * pixelBytes,
							matrixRow.col(rangeStart - regionLeft).data,
							bytesToCopy);
					}
				}
			}
		}
	}

	// if entire image is used, no further emplacementes needed
	return true;
}


} // namespace iocv


