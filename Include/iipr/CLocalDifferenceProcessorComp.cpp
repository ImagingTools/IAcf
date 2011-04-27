#include "iipr/CLocalDifferenceProcessorComp.h"


#include "iimg/CGeneralBitmap.h"


namespace iipr
{


// reimplemented (iipr::CImageProcessorCompBase)

bool CLocalDifferenceProcessorComp::ProcessImage(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	if ((inputImage.GetComponentsCount() != 1) || (inputImage.GetComponentBitsCount() != 8)){
		return false;	// input image format is not supported
	}

	istd::CIndex2d inputImageSize = inputImage.GetImageSize();
	if (!outputImage.CreateBitmap(inputImage.GetPixelFormat(), inputImageSize)){
		return false;	// cannot create output image
	}

	const iprm::ILinearAdjustParams* adjustParamsPtr = m_defaultAdjustParamsCompPtr.GetPtr();
	const IMultidimensionalFilterParams* filterParamsPtr = m_defaultFilterParamCompPtr.GetPtr();

	if (paramsPtr != NULL){
		if (m_adjustParamsIdAttrPtr.IsValid()){
			adjustParamsPtr = dynamic_cast<const iprm::ILinearAdjustParams*>(paramsPtr->GetParameter((*m_adjustParamsIdAttrPtr).ToString()));
		}
		if (m_filterParamIdAttrPtr.IsValid()){
			filterParamsPtr = dynamic_cast<const IMultidimensionalFilterParams*>(paramsPtr->GetParameter((*m_filterParamIdAttrPtr).ToString()));
		}
	}

	double contrast = 1;
	double brightness = 128;
	if (adjustParamsPtr != NULL){
		contrast = adjustParamsPtr->GetScaleFactor();
		brightness = adjustParamsPtr->GetOffsetFactor() * 255;
	}

	int filterLength = 10;
	if (filterParamsPtr != NULL){
		imath::CVarVector filters = filterParamsPtr->GetFilterLengths();
		if (filters.GetElementsCount() >= 1){
			filterLength = int(filters[0] + 0.5);
		}
	}

	int imageHeight = inputImageSize.GetY();
	int imageWidth = inputImageSize.GetX();

	for (int lineIndex = 0; lineIndex < imageHeight; ++lineIndex){
		I_BYTE* inputLinePtr = (I_BYTE*)inputImage.GetLinePtr(lineIndex);
		I_BYTE* outputLinePtr = (I_BYTE*)outputImage.GetLinePtr(lineIndex);

		for (int x = 0; x < imageWidth; x++){
			int beginPoint = x - filterLength;
			if (beginPoint < 0){
				beginPoint = 0;
			}
			int endPoint = x + filterLength;
			if (endPoint >= imageWidth){
				endPoint = imageWidth - 1;
			}

			I_BYTE firstValue = *(inputLinePtr + beginPoint);
			I_BYTE lastValue = *(inputLinePtr + endPoint);
 
			I_BYTE* outputImagePtr = outputLinePtr + x;
			int outputValue = int(labs(firstValue - lastValue) * contrast + brightness);
			if (outputValue < 0){
				outputValue = 0;
			}
			else if (outputValue > 255){
				outputValue = 255;
			}

			*outputImagePtr = I_BYTE(outputValue);
		}
	}
	
	return true;
}


} // namespace iipr

