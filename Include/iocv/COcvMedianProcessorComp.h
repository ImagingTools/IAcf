#pragma once


// ACF-Solutions includes
#include <iipr/CImageRegionProcessorCompBase.h>


namespace iocv
{


/**	
	Median-based smoothing processor.
*/
class COcvMedianProcessorComp: public iproc::CSyncProcessorCompBase
{
public:
	typedef iproc::CSyncProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(COcvMedianProcessorComp);
		I_ASSIGN(m_filterSizeParamsIdAttrPtr, "FilterSizeParamsId", "ID of the filter dimension parameter set", true, "FilterSizeParamsId");
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual iproc::IProcessor::TaskState DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL) override;

private:
	I_ATTR(QByteArray, m_filterSizeParamsIdAttrPtr);
};


} // namespace iocv


