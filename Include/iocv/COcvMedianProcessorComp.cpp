#include <iocv/COcvMedianProcessorComp.h>


 // ACF includes
#include <istd/CIndex2d.h>
#include <ibase/CSize.h>
#include <iprm/TParamsPtr.h>
#include <iimg/IBitmap.h>

// ACF-Solitions includes
#include <imeas/INumericValue.h>

// OpenCV includes
#include <opencv2/imgproc/imgproc.hpp>


namespace iocv
{


// protected methods

// reimplemented (iproc::IProcessor)

int COcvMedianProcessorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		SendWarningMessage(0, "Input bitmap is not set");

		return false;
	}

	if (inputBitmapPtr->IsEmpty()){
		SendWarningMessage(0, "Input bitmap is empty.");

		return false;
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (outputBitmapPtr == NULL){
		SendWarningMessage(0, "Output bitmap is not set");

		return false;
	}

	istd::CIndex2d inputBitmapSize = inputBitmapPtr->GetImageSize();

	if (!outputBitmapPtr->CopyFrom(*inputBitmapPtr)){
		SendErrorMessage(0, "Data could not be copied from input bitmap to the output");

		return false;
	}

	iprm::TParamsPtr<imeas::INumericValue> filterSizePtr(paramsPtr, *m_filterSizeParamsIdAttrPtr);
	if (filterSizePtr == NULL){
		SendErrorMessage(0, "No fiter dimension was set");

		return false;
	}

	imath::CVarVector filterLengths = filterSizePtr->GetValues();
	int filterDimensionsCount = filterLengths.GetElementsCount();
	if (filterDimensionsCount < 1){
		SendErrorMessage(0, "Processing filter can't have dimension smaller 1");

		return false;
	}

	int imageWidth = inputBitmapSize.GetX();
	int imageHeight = inputBitmapSize.GetY();

	int kernelMaxWidth = qMax(1, qMin(int(filterLengths[0]), imageWidth));
	int kernelMaxHeight = qMax(1, qMin((filterDimensionsCount < 2)? kernelMaxWidth: int(filterLengths[1]), imageHeight));

	Q_ASSERT(kernelMaxWidth >= 1);
	Q_ASSERT(kernelMaxHeight >= 1);

	cv::Mat inputMatrix(inputBitmapPtr->GetImageSize().GetY(), inputBitmapPtr->GetImageSize().GetX(), CV_8UC1, (qint8*)inputBitmapPtr->GetLinePtr(0));
	cv::Mat outputMatrix(outputBitmapPtr->GetImageSize().GetY(), outputBitmapPtr->GetImageSize().GetX(), CV_8UC1, (qint8*)outputBitmapPtr->GetLinePtr(0));

	cv::_InputArray input(inputMatrix);
	cv::_OutputArray output(outputMatrix);

	cv::medianBlur(input, output, qMax(kernelMaxWidth, kernelMaxHeight));

	return true;
}


} // namespace iocv


