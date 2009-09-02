#ifndef iswr_CSwissRangerParamsComp_included
#define iswr_CSwissRangerParamsComp_included


// ACF includes
#include "icomp/CComponentBase.h"

// IACF includes
#include "iswr/CSwissRangerParams.h"


namespace iswr
{


/**
	SwissRanger based camera component implementing interfaces 
	\c icam::IBitmapAcquisition and \c icam::IExposureConstraints for exposure control support.
*/
class CSwissRangerParamsComp: public icomp::CComponentBase, public iswr::CSwissRangerParams

{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSwissRangerParamsComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(iswr::ISwissRangerParams);	
		I_ASSIGN(m_isMedianFilterEnabledAttrPtr, "MedianEnabled", "Internal 3x3 median filter is enabled", false, true);
		I_ASSIGN(m_amplitudeThresholdAttrPtr, "AmplitudeThreshold", "Threshold for signal amplitude.\nAll values below this value are ignored", false, 0);
		I_ASSIGN(m_modulationFrequencyModeAttrPtr, "ModulationFrequency", "Modulation frequency.\n0 - 40MHz(3.75m SRR3)\n1 - 30MHz(5m SR3, SR4)\n2 - 21MHz(7.14m, SR3)\n3 - 20MHz(7.5m, SR3)\n4 - 19MHz(7.89m, SR3)", false, 0);
		I_ASSIGN(m_distanceClipMinAttrPtr, "MinDistanceClip", "Distances below this value clipped to the range", false, 0);
		I_ASSIGN(m_distanceClipMaxAttrPtr, "MaxDistanceClip", "Distances above this value clipped to the range", false, 1.0);
	I_END_COMPONENT;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	I_ATTR(bool, m_isMedianFilterEnabledAttrPtr);
	I_ATTR(double, m_amplitudeThresholdAttrPtr);
	I_ATTR(int, m_modulationFrequencyModeAttrPtr);
	I_ATTR(double, m_distanceClipMinAttrPtr);
	I_ATTR(double, m_distanceClipMaxAttrPtr);
};


} // namespace iswr


#endif // !iswr_CSwissRangerParamsComp_included


