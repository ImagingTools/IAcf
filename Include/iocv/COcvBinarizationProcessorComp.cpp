// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-IAcf-Commercial
#include <iocv/COcvBinarizationProcessorComp.h>

 // ACF includes
#include <istd/CIndex2d.h>
#include <ibase/CSize.h>
#include <iprm/TParamsPtr.h>
#include <iimg/IBitmap.h>
#include <iprm/ISelectionParam.h>
#include <iprm/IEnableableParam.h>
#include <imeas/INumericValue.h>
#include <i2d/IObject2d.h>

// OpenCV includes
#include <opencv2/imgproc/imgproc.hpp>


namespace iocv
{


struct ThresholdParams {
	bool invert = false;
	bool keepIntensity = false;
	int lowThreshold = 0;
	int highThreshold = 255;
};


struct AdaptiveParams {
	bool invert = false;
	int kernelSize = 15;
	int segmentationOffset = 0;
};


static bool DoFixedBinarization(cv::Mat& inputMatrix, cv::Mat& outputMatrix, bool invert, int mode)
{
	cv::_InputArray input(inputMatrix);
	cv::_OutputArray output(outputMatrix);

	int flags = (mode == COcvBinarizationProcessorComp::BIN_OTSU) ? cv::THRESH_OTSU : cv::THRESH_TRIANGLE;

	if (invert) {
		cv::threshold(input, output, 0, 255, cv::THRESH_BINARY_INV | flags);
	}
	else {
		cv::threshold(input, output, 0, 255, cv::THRESH_BINARY | flags);
	}

	return true;
}


static bool DoThresholdBinarization(cv::Mat& inputMatrix, cv::Mat& outputMatrix, const ThresholdParams& threshParams)
{
	cv::Mat tempMatrix1, tempMatrix2;
	cv::_InputArray input(inputMatrix);
	cv::_OutputArray output(outputMatrix);

	tempMatrix1.copySize(outputMatrix);

	if (threshParams.keepIntensity) {
		// original bandpass
		if (threshParams.invert) {
			tempMatrix2.copySize(outputMatrix);

			cv::threshold(input, tempMatrix1, threshParams.lowThreshold, 255, cv::THRESH_TOZERO_INV);
			cv::threshold(input, tempMatrix2, threshParams.highThreshold, 255, cv::THRESH_TOZERO);
			cv::addWeighted(tempMatrix1, 1.0, tempMatrix2, 1.0, 0.0, outputMatrix);
		}
		else {
			cv::threshold(input, tempMatrix1, threshParams.lowThreshold, 255, cv::THRESH_TOZERO);
			cv::threshold(tempMatrix1, output, threshParams.highThreshold, 255, cv::THRESH_TOZERO_INV);
		}
	}
	else {
		if (threshParams.invert) {
			tempMatrix2.copySize(outputMatrix);

			// binary bandpass invert: 0 if x in (thr1, thr2), 255 otherwise
			cv::threshold(input, tempMatrix1, threshParams.highThreshold, 255, cv::THRESH_TOZERO_INV);
			cv::threshold(tempMatrix1, tempMatrix2, threshParams.lowThreshold, 255, cv::THRESH_BINARY);
			cv::bitwise_not(tempMatrix2, output);
		}
		else {
			// binary bandpass: 255 if x in (thr1, thr2), 0 otherwise
			cv::threshold(input, tempMatrix1, threshParams.highThreshold, 255, cv::THRESH_TOZERO_INV);	// > high ? = 0
			cv::threshold(tempMatrix1, output, threshParams.lowThreshold, 255, cv::THRESH_BINARY);		// > low ? = 255, else 0
		}
	}

	return true;
}


static bool DoAdaptiveBinarization(cv::Mat& inputMatrix, cv::Mat& outputMatrix, const AdaptiveParams& adaptiveParams)
{
	cv::_InputArray input(inputMatrix);
	cv::_OutputArray output(outputMatrix);

	cv::adaptiveThreshold(input, output,
		255,
		cv::ADAPTIVE_THRESH_GAUSSIAN_C,
		adaptiveParams.invert ? cv::THRESH_BINARY_INV : cv::THRESH_BINARY,
		adaptiveParams.kernelSize,
		adaptiveParams.segmentationOffset);

	return true;
}


// protected methods

// reimplemented (iproc::IProcessor)

iproc::IProcessor::TaskState COcvBinarizationProcessorComp::DoProcessing(
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

	istd::CIndex2d inputBitmapSize = inputBitmapPtr->GetImageSize();
	int imageWidth = inputBitmapSize.GetX();
	int imageHeight = inputBitmapSize.GetY();


	// Operation mode
	int operationMode = BIN_THRESHOLD;	// threshold by default
	iprm::TParamsPtr<iprm::ISelectionParam> modeParamPtr(paramsPtr, "OperationMode");
	if (modeParamPtr.IsValid()) {
		operationMode = modeParamPtr->GetSelectedOptionIndex();
		if (operationMode < 0 || operationMode >= _BIN_LAST) {
			SendErrorMessage(0, QT_TR_NOOP(QString("Wrong OperationMode passed: %1").arg(operationMode)));
			return TS_INVALID;
		}
	}


	// AOI
	cv::Mat inputMatrix, outputMatrix;
	MakeProcessedRegionMatrices(inputBitmapPtr, outputBitmapPtr, paramsPtr, inputMatrix, outputMatrix);

	// cannot do anything if AOI is empty - but maybe its ok
	if (inputMatrix.empty() || outputMatrix.empty()) {
		SendInfoMessage(0, QT_TR_NOOP("Empty image region for processing."));
		return TS_OK;
	}

	cv::_InputArray input(inputMatrix);
	cv::_OutputArray output(outputMatrix);


	// Parameters & processing
	if (operationMode == BIN_THRESHOLD) {
		ThresholdParams threshParams;

		iprm::TParamsPtr<iprm::IEnableableParam> invertPrm(paramsPtr, "Invert");
		if (invertPrm.IsValid())
			threshParams.invert = invertPrm->IsEnabled();

		iprm::TParamsPtr<iprm::IEnableableParam> keepIntensityPrm(paramsPtr, "Intensity");
		if (keepIntensityPrm.IsValid())
			threshParams.keepIntensity = keepIntensityPrm->IsEnabled();

		iprm::TParamsPtr<imeas::INumericValue> threshParamPtr(paramsPtr, "Threshold");
		if (threshParamPtr.IsValid()) {
			auto vals = threshParamPtr->GetValues();
			if (vals.GetElementsCount() == 2) {
				threshParams.lowThreshold = vals[0];
				threshParams.highThreshold = vals[1];
			}
		}

		DoThresholdBinarization(inputMatrix, outputMatrix, threshParams);
	}
	else if (operationMode == BIN_ADAPTIVE) {
		AdaptiveParams adaptiveParams;

		iprm::TParamsPtr<iprm::IEnableableParam> invertPrm(paramsPtr, "Invert");
		if (invertPrm.IsValid())
			adaptiveParams.invert = invertPrm->IsEnabled();

		iprm::TParamsPtr<imeas::INumericValue> segmentationOffsetParamPtr(paramsPtr, "SegmentationOffset");
		if (segmentationOffsetParamPtr.IsValid()) {
			adaptiveParams.segmentationOffset = 255 * segmentationOffsetParamPtr->GetValues()[0];
		}

		iprm::TParamsPtr<imeas::INumericValue> filterSizeParamPtr(paramsPtr, "FilterSize");
		if (!filterSizeParamPtr.IsValid()) {
			SendErrorMessage(0, QT_TR_NOOP("No filter dimension was set, processing failed"));
			return TS_INVALID;
		}

		int filterValue = filterSizeParamPtr->GetValues()[0];
		int kernelMaxWidth = std::max(1, std::min(filterValue, imageWidth));
		int kernelMaxHeight = std::min(kernelMaxWidth, imageHeight);
		int kernelSize = std::min(kernelMaxWidth, kernelMaxHeight);
		if ((kernelSize & 1) == 0) {
			++kernelSize;
		}

		adaptiveParams.kernelSize = kernelSize;

		DoAdaptiveBinarization(inputMatrix, outputMatrix, adaptiveParams);
	}
	else if (operationMode == BIN_OTSU || operationMode == BIN_TRIANGLE) {
		bool invert = false;

		iprm::TParamsPtr<iprm::IEnableableParam> invertPrm(paramsPtr, "Invert");
		if (invertPrm.IsValid())
			invert = invertPrm->IsEnabled();

		DoFixedBinarization(inputMatrix, outputMatrix, invert, operationMode);
	}


	// Output
	EmplaceProcessedRegionBack(outputBitmapPtr, paramsPtr, outputMatrix);

	return TS_OK;
}


} // namespace iocv


