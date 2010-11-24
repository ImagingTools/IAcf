#include "iswr/CSwissRangerParamsComp.h"


namespace iswr
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CSwissRangerParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_isMedianFilterEnabledAttrPtr.IsValid()){
		SetMedianFilterEnabled(*m_isMedianFilterEnabledAttrPtr);
	}

	if (m_amplitudeThresholdAttrPtr.IsValid()){
		SetAmplitudeThreshold(*m_amplitudeThresholdAttrPtr);
	}

	if (m_modulationFrequencyModeAttrPtr.IsValid()){
		SetModulationFrequencyMode(*m_modulationFrequencyModeAttrPtr);
	}
}


} // namespace iswr


