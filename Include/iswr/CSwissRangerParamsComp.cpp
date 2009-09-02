#include "iswr/CSwissRangerParamsComp.h"


namespace iswr
{



// reimplemented (icomp::IComponent)

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

	istd::CRange distanceClipRange(0.0, 1.0);

	if (m_distanceClipMinAttrPtr.IsValid()){
		distanceClipRange.SetMinValue(*m_distanceClipMinAttrPtr);
	}
	
	if (m_distanceClipMaxAttrPtr.IsValid()){
		distanceClipRange.SetMaxValue(*m_distanceClipMaxAttrPtr);
	}

	SetDistanceClipRange(distanceClipRange);
}


} // namespace iswr


