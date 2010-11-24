#ifndef icntl_CEjectorParamsComp_included
#define icntl_CEjectorParamsComp_included


// ACF includes
#include "icomp/CComponentBase.h"
#include "ibase/TEnableableWrap.h"
#include "ibase/TNamedWrap.h"

#include "icntl/IEjectorParams.h"


namespace icntl
{


/**
	Component implementation of single ejector parameters.
*/
class CEjectorParamsComp:
			public icomp::CComponentBase,
			public ibase::TEnableableWrap<ibase::TNamedWrap<IEjectorParams> >
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CEjectorParamsComp);
		I_REGISTER_INTERFACE(IEjectorParams);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(istd::IEnableable);
		I_REGISTER_INTERFACE(istd::INamed);
		I_REGISTER_INTERFACE(istd::IChangeable);

		I_ASSIGN(m_nameAttrPtr, "Name", "Name", true, "");
		I_ASSIGN(m_isEnabledAttrPtr, "IsEnabled", "Is function enabled", true, true);
		I_ASSIGN(m_ejectorPositionAttrPtr, "EjectorPosition", "Position of ejector in logical units", true, 1000);
		I_ASSIGN(m_ejectionDistanceAttrPtr, "EjectionDistance", "Distance of ejection in logical units", true, 50);
	I_END_COMPONENT;

	CEjectorParamsComp();

	// reimplemented (icntl::IEjectorParams)
	virtual double GetEjectorPosition() const;
	virtual void SetEjectorPosition(double position);
	virtual double GetEjectionDistance() const;
	virtual void SetEjectionDistance(double distance);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(istd::CString, m_nameAttrPtr);
	I_ATTR(bool, m_isEnabledAttrPtr);
	I_ATTR(double, m_ejectorPositionAttrPtr);
	I_ATTR(double, m_ejectionDistanceAttrPtr);

	double m_ejectorPosition;
	double m_ejectionDistance;
};


} // namespace icntl


#endif // !icntl_CEjectorParamsComp_included


