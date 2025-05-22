#pragma once


// ACF-Solutions includes
#include <iocv/COcvProcessorCompBase.h>


namespace iocv
{


/**
	Adaptive double threshold binarization processor.
*/
class COcvBandPassBinarizationComp: public iocv::COcvProcessorCompBase
{
public:
	typedef iocv::COcvProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(COcvBandPassBinarizationComp);
		I_ASSIGN(m_thresholdLowParamsIdAttrPtr, "ThresholdLowParamsId", "ID of the threshold low parameter in the parameter set", true, "ThresholdLow");
		I_ASSIGN(m_thresholdHighIdAttrPtr, "ThresholdHighParamsId", "ID of the threshold high parameter in the parameter set", true, "ThresholdHigh");
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual iproc::IProcessor::TaskState DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL) override;

private:
	I_ATTR(QByteArray, m_thresholdLowParamsIdAttrPtr);
	I_ATTR(QByteArray, m_thresholdHighIdAttrPtr);
};


} // namespace iocv


