#ifndef icntl_CInspectionUnitParamsComp_included
#define icntl_CInspectionUnitParamsComp_included


// ACF includes
#include "icomp/CComponentBase.h"
#include "ibase/TEnableableWrap.h"
#include "ibase/TNamedWrap.h"

#include "icntl/IInspectionUnitParams.h"


namespace icntl
{


/**
	Parameters of single inspection unit.
*/
class CInspectionUnitParamsComp:
			public icomp::CComponentBase,
			public ibase::TEnableableWrap<ibase::TNamedWrap<IInspectionUnitParams> >
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CInspectionUnitParamsComp);
		I_REGISTER_INTERFACE(IInspectionUnitParams);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(istd::IEnableable);
		I_REGISTER_INTERFACE(istd::INamed);
		I_REGISTER_INTERFACE(istd::IChangeable);

		I_ASSIGN(m_nameAttrPtr, "Name", "Name", true, "");
		I_ASSIGN(m_isEnabledAttrPtr, "IsEnabled", "Is function enabled", true, true);
		I_ASSIGN(m_triggerOffsetAttrPtr, "TriggerOffset", "Distance between light barrier trigger and real trigger position", true, 0);
		I_ASSIGN(m_triggerAlphaAttrPtr, "TriggerAlpha", "0 if rising edge should be used, 1 if falling and rest values are interpolated", true, 0);
	I_END_COMPONENT;

	CInspectionUnitParamsComp();

	// reimplemented (icntl::IInspectionUnitParams)
	virtual double GetTriggerOffset() const;
	virtual void SetTriggerOffset(double offset);
	virtual double GetTriggerAlpha() const;
	virtual void SetTriggerAlpha(double value);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	I_ATTR(istd::CString, m_nameAttrPtr);
	I_ATTR(bool, m_isEnabledAttrPtr);
	I_ATTR(double, m_triggerOffsetAttrPtr);
	I_ATTR(double, m_triggerAlphaAttrPtr);

	double m_triggerOffset;
	double m_triggerAlpha;
};


} // namespace icntl


#endif // !icntl_CInspectionUnitParamsComp_included


