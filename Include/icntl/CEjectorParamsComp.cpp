#include "icntl/CEjectorParamsComp.h"


namespace icntl
{


CEjectorParamsComp::CEjectorParamsComp()
:	m_ejectorPosition(1000),
	m_ejectionDistance(50)
{
}


// reimplemented (icntl::IEjectorParams)

double CEjectorParamsComp::GetEjectorPosition() const
{
	return m_ejectorPosition;
}


void CEjectorParamsComp::SetEjectorPosition(double position)
{
	if (position != m_ejectorPosition){
		istd::CChangeNotifier notifier(this);

		m_ejectorPosition = position;
	}
}


double CEjectorParamsComp::GetEjectionDistance() const
{
	return m_ejectionDistance;
}


void CEjectorParamsComp::SetEjectionDistance(double distance)
{
	if (distance != m_ejectionDistance){
		istd::CChangeNotifier notifier(this);

		m_ejectionDistance = distance;
	}
}


// reimplemented (iser::ISerializable)

bool CEjectorParamsComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	retVal = retVal && SerializeName(archive);
	retVal = retVal && SerializeEnabled(archive);

	static iser::CArchiveTag ejectorPositionTag("EjectorPosition", "Position of ejector in logical units");
	retVal = retVal && archive.BeginTag(ejectorPositionTag);
	retVal = retVal && archive.Process(m_ejectorPosition);
	retVal = retVal && archive.EndTag(ejectorPositionTag);

	static iser::CArchiveTag ejectionDistanceTag("EjectionDistance", "Distance ejector is on in logical units");
	retVal = retVal && archive.BeginTag(ejectionDistanceTag);
	retVal = retVal && archive.Process(m_ejectionDistance);
	retVal = retVal && archive.EndTag(ejectionDistanceTag);

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CEjectorParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetName(*m_nameAttrPtr);
	SetEnabled(*m_isEnabledAttrPtr);

	m_ejectorPosition = *m_ejectorPositionAttrPtr;
	m_ejectionDistance = *m_ejectionDistanceAttrPtr;
}


} // namespace icntl


