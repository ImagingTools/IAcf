#pragma once


// ACF-Solutions includes
#include <iipr/CImageRegionProcessorCompBase.h>


namespace iocv
{


/**	
	Non-local Means Denoising algorithm
*/
class COcvDenoisingProcessorComp : public iproc::CSyncProcessorCompBase
{
public:
	typedef iproc::CSyncProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(COcvDenoisingProcessorComp);
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);

private:
};


} // namespace iocv


