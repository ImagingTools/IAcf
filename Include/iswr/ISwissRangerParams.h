#ifndef iswr_ISwissRangerParams_included
#define iswr_ISwissRangerParams_included


#include "iswr/iswr.h"

// ACF includes
#include "iser/ISerializable.h"


namespace iswr
{


/**
	Parameters od SwissRanger sensor
*/
class ISwissRangerParams: virtual public iser::ISerializable
{
public:
	virtual void SetMedianFilterEnabled(bool isEnabled = true) = 0;
	virtual bool GetMedianFilterEnabled() const = 0;

	virtual void SetAmplitudeThreshold(double amplitudeThreshold) = 0;
	virtual double GetAmplitudeThreshold() const = 0;

	virtual void SetModulationFrequencyMode(int modulationFrequencyMode) = 0;
	virtual int GetModulationFrequencyMode() const = 0;
};


} // namespace iswr


#endif // !iswr_ISwissRangerParams_included


