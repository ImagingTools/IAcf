#include "iswr/CSwissRangerParams.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


// SwissRanger includes
#include <windows.h>
#include <libMesaSR.h>


namespace iswr
{


// public methods

CSwissRangerParams::CSwissRangerParams()
	:m_isMedianFilterEnabled(true),
	m_isAdaptiveFilterEnabled(true),
	m_amplitudeThreshold(0.0),
	m_modulationFrequencyMode(MF_31MHz)
{
}


// reimplemented (iswr::ISwissRangerParams)

void CSwissRangerParams::SetMedianFilterEnabled(bool isMedianFilterEnabled)
{
	if (m_isMedianFilterEnabled != isMedianFilterEnabled){
		istd::CChangeNotifier changePtr(this);

		m_isMedianFilterEnabled = isMedianFilterEnabled;
	}
}


bool CSwissRangerParams::IsMedianFilterEnabled() const
{
	return m_isMedianFilterEnabled;
}


void CSwissRangerParams::SetAdaptiveFilterEnabled(bool isAdaptiveFilterEnabled)
{
	if (m_isAdaptiveFilterEnabled != isAdaptiveFilterEnabled){
		istd::CChangeNotifier changePtr(this);

		m_isAdaptiveFilterEnabled = isAdaptiveFilterEnabled;
	}
}


bool CSwissRangerParams::IsAdaptiveFilterEnabled() const
{
	return m_isAdaptiveFilterEnabled;
}


void CSwissRangerParams::SetAmplitudeThreshold(double amplitudeThreshold)
{
	if (amplitudeThreshold != m_amplitudeThreshold){
		istd::CChangeNotifier changePtr(this);

		m_amplitudeThreshold = amplitudeThreshold;
	}
}


double CSwissRangerParams::GetAmplitudeThreshold() const
{
	return m_amplitudeThreshold;
}


void CSwissRangerParams::SetModulationFrequencyMode(int modulationFrequencyMode)
{
	if (modulationFrequencyMode != m_modulationFrequencyMode){
		istd::CChangeNotifier changePtr(this);

		m_modulationFrequencyMode = modulationFrequencyMode;
	}
}


int CSwissRangerParams::GetModulationFrequencyMode() const
{
	return m_modulationFrequencyMode;
}


// reimplemented (iser::ISerializable)


bool CSwissRangerParams::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag isMedianFilterEnabledTag("MedianFilterEnabled", "MedianFilterEnabled");
	retVal = retVal && archive.BeginTag(isMedianFilterEnabledTag);
	retVal = retVal && archive.Process(m_isMedianFilterEnabled);
	retVal = retVal && archive.EndTag(isMedianFilterEnabledTag);

	static iser::CArchiveTag isAdaptiveFilterEnabledTag("AdaptiveFilterEnabled", "AdaptiveFilterEnabled");
	retVal = retVal && archive.BeginTag(isAdaptiveFilterEnabledTag);
	retVal = retVal && archive.Process(m_isAdaptiveFilterEnabled);
	retVal = retVal && archive.EndTag(isAdaptiveFilterEnabledTag);

	static iser::CArchiveTag amplitudeThresholdTag("AmplitudeThreshold", "AmplitudeThreshold");
	retVal = retVal && archive.BeginTag(amplitudeThresholdTag);
	retVal = retVal && archive.Process(m_amplitudeThreshold);
	retVal = retVal && archive.EndTag(amplitudeThresholdTag);

	static iser::CArchiveTag modulationFrequencyModeTag("ModulationFrequencyMode", "ModulationFrequencyMode");
	retVal = retVal && archive.BeginTag(modulationFrequencyModeTag);
	retVal = retVal && archive.Process(m_modulationFrequencyMode);
	retVal = retVal && archive.EndTag(modulationFrequencyModeTag);

	return retVal;
}


} // namespace iswr


