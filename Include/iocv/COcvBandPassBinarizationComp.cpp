#include <iocv/COcvBandPassBinarizationComp.h>


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

iproc::IProcessor::TaskState COcvBandPassBinarizationComp::DoProcessing(
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
	int lowThreshold = 0;
	iprm::TParamsPtr<imeas::INumericValue> lowThreshParamPtr(paramsPtr, *m_thresholdLowParamsIdAttrPtr);
	if (!lowThreshParamPtr.IsValid()){
		SendErrorMessage(0, "No low threshold param was set, processing failed");

		return TS_INVALID;
	}
	lowThreshold = lowThreshParamPtr->GetValues()[0];
	
	int highThreshold = 255;
	iprm::TParamsPtr<imeas::INumericValue> highThreshParamPtr(paramsPtr, *m_thresholdHighIdAttrPtr);
	if (highThreshParamPtr.IsValid()) {
		highThreshold = highThreshParamPtr->GetValues()[0];
	}


	bool invert = false;
	iprm::TParamsPtr<iprm::IEnableableParam> invertPrm(paramsPtr, "Invert");
	if (invertPrm.IsValid())
		invert = invertPrm->IsEnabled();

	bool keepIntensity = true;
	iprm::TParamsPtr<iprm::IEnableableParam> keepIntensityPrm(paramsPtr, "Intensity");
	if (keepIntensityPrm.IsValid())
		keepIntensity = keepIntensityPrm->IsEnabled();

	
	// AOI
	cv::Mat inputMatrix, outputMatrix;

	MakeProcessedRegionMatrices(inputBitmapPtr, outputBitmapPtr, paramsPtr, inputMatrix, outputMatrix);

	if (inputMatrix.empty() || outputMatrix.empty()) {
		SendInfoMessage(0, "Empty image region for processing. Exit routine", "OcvBandPassBinarization");
		return TS_OK;
	}


	// Processing
	cv::Mat tempMatrix1, tempMatrix2;
	cv::_InputArray input(inputMatrix);
	cv::_OutputArray output(outputMatrix);

	double maxVal = 255;
	tempMatrix1.copySize(outputMatrix);

	if (keepIntensity) {
		// original bandpass
		if (invert) {
			tempMatrix2.copySize(outputMatrix);

			cv::threshold(input, tempMatrix1, lowThreshold, maxVal, cv::THRESH_TOZERO_INV);
			cv::threshold(input, tempMatrix2, highThreshold, maxVal, cv::THRESH_TOZERO);
			cv::addWeighted(tempMatrix1, 1.0, tempMatrix2, 1.0, 0.0, outputMatrix);
		}
		else {
			cv::threshold(input, tempMatrix1, lowThreshold, maxVal, cv::THRESH_TOZERO);
			cv::threshold(tempMatrix1, output, highThreshold, maxVal, cv::THRESH_TOZERO_INV);
		}
	}
	else {
		if (invert) {
			tempMatrix2.copySize(outputMatrix);

			// binary bandpass invert: 0 if x in (thr1, thr2), 255 otherwise
			cv::threshold(input, tempMatrix1, highThreshold, maxVal, cv::THRESH_TOZERO_INV);
			cv::threshold(tempMatrix1, tempMatrix2, lowThreshold, maxVal, cv::THRESH_BINARY);
			cv::bitwise_not(tempMatrix2, output);
		}
		else {
			// binary bandpass: 255 if x in (thr1, thr2), 0 otherwise
			cv::threshold(input, tempMatrix1, highThreshold, maxVal, cv::THRESH_TOZERO_INV);	// > high ? = 0
			cv::threshold(tempMatrix1, output, lowThreshold, maxVal, cv::THRESH_BINARY);		// > low ? = 255, else 0
		}
	}
	
	EmplaceProcessedRegionBack(outputBitmapPtr, paramsPtr, outputMatrix);

	return TS_OK;
}


} // namespace iocv


