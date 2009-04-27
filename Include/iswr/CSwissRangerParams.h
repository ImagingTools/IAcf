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
	virtual void SetAmplitudeThreshold(double amplitudeThreshold);
	virtual double GetAmplitudeThreshold() const;
	virtual void SetModulationFrequencyMode(int modulationFrequencyMode);
	virtual int GetModulationFrequencyMode() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	bool m_isMedianFilterEnabled;
	double m_amplitudeThreshold;
	int m_modulationFrequencyMode;
};


} // namespace iswr


#endif // !iswr_CSwissRangerParams_included


