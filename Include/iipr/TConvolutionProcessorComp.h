#ifndef iipr_TConvolutionProcessorComp_included
#define iipr_TConvolutionProcessorComp_included


#include "iipr/TImageParamProcessorCompBase.h"

#include "iipr/CConvolutionKernel2d.h"


namespace iipr
{


/**
	Implementation of general convolution operator.
*/
template <typename PixelType, class ParameterType> 
class TConvolutionProcessorComp: public iipr::TImageParamProcessorCompBase<ParameterType>
{
public:
	typedef iipr::TImageParamProcessorCompBase<ParameterType> BaseClass;

protected:		
	// abstract methods
	virtual CConvolutionKernel2d* CreateKernel(const ParameterType* paramsPtr) = 0;

	// reimplemented (iipr::TImageParamProcessorCompBase<ParameterType>)
	virtual bool ParamProcessImage(
				const ParameterType* paramsPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);
};


// reimplemented (iipr::TImageParamProcessorCompBase<ParameterType>)

template <typename PixelType, class ParameterType> 
bool TConvolutionProcessorComp<PixelType, ParameterType, Kernel>::ParamProcessImage(
			const ParameterType* paramsPtr,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	if (paramsPtr == NULL){
		return false;
	}

	bool retVal = true;

	istd::TDelPtr<CConvolutionKernel2d> filterKernelPtr(CreateKernel(paramsPtr));
	if (!filterKernelPtr.IsValid()){
		return false;
	}

	// initialize kernel with input image:
	filterKernelPtr->InitForBitmap(inputImage);

	outputImage

	return retVal;
}


} // namespace iipr


#endif // !iipr_TConvolutionProcessorComp_included

