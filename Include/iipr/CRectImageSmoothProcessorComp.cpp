#include "iipr/CRectImageSmoothProcessorComp.h"


// ACF includes
#include "imath/CFixedPointManip.h"
#include "iimg/CGeneralBitmap.h"


namespace iipr
{


// reimplemented (iipr::IMultidimensionalFilterConstraints)

int CRectImageSmoothProcessorComp::GetFilterDimensionsCount() const
{
	return 2;
}


istd::CRange CRectImageSmoothProcessorComp::GetFilterLengthRange(int /*dimension*/) const
{
	return istd::CRange(1, 100);
}


const imath::IDoubleManip& CRectImageSmoothProcessorComp::GetFilterLengthManip(int /*dimension*/) const
{
	static imath::CFixedPointManip manip(0);

	return manip;
}


// reimplemented (iipr::TImageParamProcessorCompBase<iipr::IMultidimensionalFilterParams>)

bool CRectImageSmoothProcessorComp::ParamProcessImage(
			const IMultidimensionalFilterParams* paramsPtr,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	if (paramsPtr == NULL){
		return false;
	}

	if (inputImage.GetComponentBitsCount() != 8){
		return false;	// Bad input image format
	}

	int componentsCount = inputImage.GetComponentsCount();
	I_ASSERT(inputImage.GetPixelBitsCount() == componentsCount * 8);

	istd::CIndex2d imageSize = inputImage.GetImageSize();
	if (imageSize.IsSizeEmpty()){
		return false;
	}

	if (!outputImage.CreateBitmap(imageSize, componentsCount * 8, componentsCount)){
		return false;	// cannot create output image
	}

	imath::CVarVector filterLengths = paramsPtr->GetFilterLengths();
	int filterDimensionsCount = filterLengths.GetElementsCount();
	if (filterDimensionsCount < 1){
		return false;
	}

	int kernelHalfWidth = int(filterLengths[0] * 0.5 - 0.5);
	int kernelHalfHeight = (filterDimensionsCount < 2)? kernelHalfWidth: int(filterLengths[1] * 0.5 - 0.5);
	int kernelHeight = kernelHalfHeight * 2 + 1;

	int imageWidth = imageSize.GetX();
	int imageHeight = imageSize.GetY();

	if (kernelHalfWidth > 0){
		for (int lineIndex = 0; lineIndex < imageHeight; ++lineIndex){
			I_BYTE* outputPtr = (I_BYTE*)outputImage.GetLinePtr(lineIndex);

			const I_BYTE* inputLinePtr = (const I_BYTE*)inputImage.GetLinePtr(lineIndex);
			const I_BYTE* inputLineEndPtr = inputLinePtr + imageWidth;

			const I_BYTE* posInputPtr = inputLinePtr;

			int kernelWidth = 0;
			int meanValue = 0;

			for (int x = 0; (x < kernelHalfWidth) && (posInputPtr < inputLineEndPtr); ++x){
				meanValue += *posInputPtr++;
				++kernelWidth;

				*outputPtr++ = I_BYTE((meanValue + 0.5) / kernelWidth);

				if (posInputPtr >= inputLineEndPtr){
					break;
				}

				meanValue += *posInputPtr++;
				++kernelWidth;
			}

			++kernelWidth;

			const I_BYTE* negInputPtr = inputLinePtr;

			while (posInputPtr < inputLineEndPtr){
				meanValue += *posInputPtr++;
				*outputPtr++ = I_BYTE((meanValue + 0.5) / kernelWidth);
				meanValue -= *negInputPtr++;
			}

			--kernelWidth;

			while (negInputPtr < inputLineEndPtr){
				meanValue -= *negInputPtr++;
				--kernelWidth;

				*outputPtr++ = I_BYTE((meanValue + 0.5) / kernelWidth);

				if  (negInputPtr >= inputLineEndPtr){
					break;
				}

				meanValue -= *negInputPtr++;
				--kernelWidth;
			}
		}
	}

	if (kernelHalfHeight > 0){
		iimg::CGeneralBitmap tempBitmap;
		tempBitmap.CopyFrom(outputImage);

		int inputLinesDifference = tempBitmap.GetLinesDifference();
		int outputLinesDifference = outputImage.GetLinesDifference();

		for (int x = 0; x < imageWidth; ++x){	
			double meanValue = 0;
			I_BYTE* inputLinePtr = ((I_BYTE*)tempBitmap.GetLinePtr(0)) + x;

			for (int y = 0; y < kernelHeight; ++y){
				meanValue += inputLinePtr[y * inputLinesDifference];
			}

			*((I_BYTE*)(outputImage.GetLinePtr(kernelHalfHeight)) + x) = I_BYTE(meanValue / kernelHeight);

			I_BYTE* outputBufferPtr = (I_BYTE*)(outputImage.GetLinePtr(kernelHalfHeight + 1)) + x;
			I_BYTE* beginKernelPtr = inputLinePtr;
			I_BYTE* endKernelPtr = inputLinePtr + kernelHeight * inputLinesDifference;
			I_BYTE* endBufferPtr = inputLinePtr + imageHeight * inputLinesDifference;

			while (endKernelPtr < endBufferPtr){
				meanValue += (*endKernelPtr - *beginKernelPtr);
				*outputBufferPtr = I_BYTE(meanValue / kernelHeight);

				endKernelPtr += inputLinesDifference;
				beginKernelPtr += inputLinesDifference;
				outputBufferPtr += outputLinesDifference;
			}
		}
	}

	return true;
}


} // namespace iipr

