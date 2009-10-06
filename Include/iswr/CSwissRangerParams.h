#ifndef iswr_CSwissRangerParams_included
#define iswr_CSwissRangerParams_included


#include "iswr/ISwissRangerParams.h"


namespace iswr
{


/**
	Parameters od SwissRanger sensor
*/
class CSwissRangerParams: virtual public iswr::ISwissRangerParams
{
public:
	CSwissRangerParams();

	// reimplemented (iswr::ISwissRangerParams)
	virtual void SetMedianFilterEnabled(bool isMedianFilterEnabled = true);
	virtual bool IsMedianFilterEnabled() const;
	virtual void SetAdaptiveFilterEnabled(bool isAdaptiveFilterEnabled = true);
	virtual bool IsAdaptiveFilterEnabled() const;
	virtual void SetAmplitudeThreshold(double amplitudeThreshold);
	virtual double GetAmplitudeThreshold() const;
	virtual void SetModulationFrequencyMode(int modulationFrequencyMode);
	virtual int GetModulationFrequencyMode() const;
	virtual void SetDistanceClipRange(const istd::CRange& distanceClipRange);
	virtual const istd::CRange& GetDistanceClipRange() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	bool m_isMedianFilterEnabled;
	bool m_isAdaptiveFilterEnabled;	
	double m_amplitudeThreshold;
	int m_modulationFrequencyMode;
	istd::CRange m_distanceClipRange;
};


} // namespace iswr


#endif // !iswr_CSwissRangerParams_included


