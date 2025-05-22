#pragma once


// ACF-Solutions includes
#include <imeas/INumericValue.h>
#include <iipr/CImageRegionProcessorCompBase.h>
#include <icalib/CPerspectiveCalibration2d.h>


namespace iocv
{


/**	
	Dnn upresampling processor
*/

class COcvDnnUpsamplingProcessorComp : public iproc::CSyncProcessorCompBase
{
public:
	typedef iproc::CSyncProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(COcvDnnUpsamplingProcessorComp);
		I_ASSIGN(m_scaleParamIdAttrPtr, "ScaleParamId", "ID of the scale parameter in the parameter set", true, "Scale");
		I_ASSIGN(m_defaultScaleCompPtr, "DefaultScale", "Default scale parameter, if not taken from parameter set", false, "DefaultScale");
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual iproc::IProcessor::TaskState DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL) override;

private:
	I_ATTR(QByteArray, m_scaleParamIdAttrPtr);
	I_REF(imeas::INumericValue, m_defaultScaleCompPtr);


	icalib::CPerspectiveCalibration2d m_perspectiveCalibration;
};


} // namespace iocv


