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
	m_modulationFrequencyMode(MF_31MHz),
	m_distanceClipRange(0.0, 1.0)
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


void CSwissRangerParams::SetDistanceClipRange(const istd::CRange& distanceClipRange)
{
	if (distanceClipRange != m_distanceClipRange){
		istd::CChangeNotifier changePtr(this);

		m_distanceClipRange = distanceClipRange;
	}
}


const istd::CRange& CSwissRangerParams::GetDistanceClipRange() const
{
	return m_distanceClipRange;
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

	double distanceClipMin = m_distanceClipRange.GetMinValue();
	static iser::CArchiveTag distanceClipMinTag("DistanceClipMin", "DistanceClipMin");
	retVal = retVal && archive.BeginTag(distanceClipMinTag);
	retVal = retVal && archive.Process(distanceClipMin);
	retVal = retVal && archive.EndTag(distanceClipMinTag);

	double distanceClipMax = m_distanceClipRange.GetMaxValue();
	static iser::CArchiveTag distanceClipMaxTag("DistanceClipMax", "DistanceClipMax");
	retVal = retVal && archive.BeginTag(distanceClipMaxTag);
	retVal = retVal && archive.Process(distanceClipMax);
	retVal = retVal && archive.EndTag(distanceClipMaxTag);

	if (!archive.IsStoring()){
		m_distanceClipRange = istd::CRange(distanceClipMin, distanceClipMax);
	}

	return retVal;
}


} // namespace iswr


