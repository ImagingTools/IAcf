// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-IAcf-Commercial
#include <iocv/COcvDenoisingProcessorComp.h>

#include <iocv/COcvImage.h>

 // ACF includes
#include <istd/CIndex2d.h>
#include <ibase/CSize.h>
#include <iprm/TParamsPtr.h>
#include <iimg/IBitmap.h>
#include <iimg/CBitmap.h>

// ACF-Solitions includes
#include <imeas/INumericValue.h>

// OpenCV includes
#include <opencv2/photo.hpp>

namespace iocv
{


// protected methods

// reimplemented (iproc::IProcessor)

iproc::IProcessor::TaskState COcvDenoisingProcessorComp::DoProcessing(
			const iprm::IParamsSet* /*paramsPtr*/,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		SendWarningMessage(0, "Input bitmap is not set");

		return TS_INVALID;
	}

	if (inputBitmapPtr->IsEmpty()){
		SendWarningMessage(0, "Input bitmap is empty.");

		return TS_INVALID;
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (outputBitmapPtr == NULL){
		SendWarningMessage(0, "Output bitmap is not set");

		return TS_INVALID;
	}

	istd::CIndex2d inputBitmapSize = inputBitmapPtr->GetImageSize();

	int imageWidth = inputBitmapSize.GetX();
	int imageHeight = inputBitmapSize.GetY();

	cv::Mat inputMatrix;
	
	if (!COcvImage::ConvertFromBitmap(*inputBitmapPtr, inputMatrix)) {
		return TS_INVALID;
	}

	cv::Mat outputMatrix(imageWidth, imageHeight, CV_8UC1, inputBitmapPtr->GetLineBytesCount());
	cv::fastNlMeansDenoising(inputMatrix, outputMatrix);

	if (!COcvImage::ConvertToBitmap(outputMatrix, *outputBitmapPtr))
	{
		return TS_INVALID;
	}

	return TS_OK;
}


} // namespace iocv


