#include <iocv/COcvAdaptiveBinarizationComp.h>


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


// protected methods

// reimplemented (iproc::IProcessor)

iproc::IProcessor::TaskState COcvAdaptiveBinarizationComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	// IO
	const iimg::IBitmap* inputBitmapPtr = nullptr;
	iimg::IBitmap* outputBitmapPtr = nullptr;
	if (!GetBitmaps(inputPtr, outputPtr, inputBitmapPtr, outputBitmapPtr)) {
		return TS_INVALID;
	}

	// Parameters
	iprm::TParamsPtr<imeas::INumericValue> filterSizeParamPtr(paramsPtr, *m_filterSizeParamsIdAttrPtr);
	if (!filterSizeParamPtr.IsValid()){
		SendErrorMessage(0, "No filter dimension was set, processing failed");

		return TS_INVALID;
	}

	imath::CVarVector filterLengths = filterSizeParamPtr->GetValues();
	int filterDimensionsCount = filterLengths.GetElementsCount();
	if (filterDimensionsCount < 1){
		SendErrorMessage(0, "Processing filter can't have dimension smaller 1");

		return TS_INVALID;
	}

	bool invert = false;
	iprm::TParamsPtr<iprm::IEnableableParam> invertPrm(paramsPtr, "Invert");
	if (invertPrm.IsValid())
		invert = invertPrm->IsEnabled();

	int segmentationOffset = 0;
	iprm::TParamsPtr<imeas::INumericValue> segmentationOffsetParamPtr(paramsPtr, *m_segmentationOffsetIdAttrPtr);
	if (segmentationOffsetParamPtr.IsValid()) {
		segmentationOffset = 255 * segmentationOffsetParamPtr->GetValues()[0];
	}

	istd::CIndex2d inputBitmapSize = inputBitmapPtr->GetImageSize();
	int imageWidth = inputBitmapSize.GetX();
	int imageHeight = inputBitmapSize.GetY();

	int kernelMaxWidth = qMax(1, qMin(int(filterLengths[0]), imageWidth));
	int kernelMaxHeight = qMax(1, qMin((filterDimensionsCount < 2)? kernelMaxWidth: int(filterLengths[1]), imageHeight));
	Q_ASSERT(kernelMaxWidth >= 1);
	Q_ASSERT(kernelMaxHeight >= 1);

	int kernelSize =  qMax(kernelMaxWidth, kernelMaxHeight);
	if ((kernelSize & 1) == 0){
		++kernelSize;
	}

	// AOI
	cv::Mat inputMatrix, outputMatrix;

	MakeProcessedRegionMatrices(inputBitmapPtr, outputBitmapPtr, paramsPtr, inputMatrix, outputMatrix);

	if (inputMatrix.empty() || outputMatrix.empty()){
		SendInfoMessage(0, "Empty image region for processing. Exit routine", "OcvAdaptiveBinarization");
		return TS_OK;
	}

	// Process
	cv::_InputArray input(inputMatrix);
	cv::_OutputArray output(outputMatrix);

	cv::adaptiveThreshold(input, output, 
				255, 
				cv::ADAPTIVE_THRESH_GAUSSIAN_C, 
				invert ? cv::THRESH_BINARY_INV : cv::THRESH_BINARY,
				kernelSize, 
				segmentationOffset);

	// Output
	EmplaceProcessedRegionBack(outputBitmapPtr, paramsPtr, outputMatrix);

	return TS_OK;
}


} // namespace iocv


