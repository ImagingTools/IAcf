#include "icntl/CLineParamsComp.h"


#include "icntl/CInspectionUnitParamsComp.h"
#include "icntl/CEjectorParamsComp.h"


namespace icntl
{


CLineParamsComp::CLineParamsComp()
:	m_ticksPerDistanceUnit(1),
	m_minObjectSize(10),
	m_positionTolerance(10)
{
}


// reimplemented (icntl::ILineParams)

int CLineParamsComp::GetInspectionUnitsCount() const
{
	return m_inspectionUnitsCompPtr.GetCount();
}


IInspectionUnitParams& CLineParamsComp::GetInspectionUnitParams(int unitIndex) const
{
	IInspectionUnitParams* unitParamsPtr = m_inspectionUnitsCompPtr[unitIndex];
	if (unitParamsPtr != NULL){
		return *unitParamsPtr;
	}

	static CInspectionUnitParamsComp dummyUnit;

	return dummyUnit;
}


int CLineParamsComp::GetEjectorsCount() const
{
	return m_ejectorsCompPtr.GetCount();
}


IEjectorParams& CLineParamsComp::GetEjectorParams(int ejectorIndex) const
{
	IEjectorParams* ejectorParamsPtr = m_ejectorsCompPtr[ejectorIndex];
	if (ejectorParamsPtr != NULL){
		return *ejectorParamsPtr;
	}

	static CEjectorParamsComp dummyEjector;

	return dummyEjector;
}


double CLineParamsComp::GetTicksPerDistanceUnit() const
{
	return m_ticksPerDistanceUnit;
}


void CLineParamsComp::SetTicksPerDistanceUnit(double value)
{
	if (value != m_ticksPerDistanceUnit){
		istd::CChangeNotifier notifier(this);

		m_ticksPerDistanceUnit = value;
	}
}


double CLineParamsComp::GetMinObjectSize() const
{
	return m_minObjectSize;
}


void CLineParamsComp::SetMinObjectSize(double value)
{
	if (value != m_minObjectSize){
		istd::CChangeNotifier notifier(this);

		m_minObjectSize = value;
	}
}


double CLineParamsComp::GetPositionTolerance() const
{
	return m_positionTolerance;
}


void CLineParamsComp::SetPositionTolerance(double value)
{
	if (value != m_positionTolerance){
		istd::CChangeNotifier notifier(this);

		m_positionTolerance = value;
	}
}


int CLineParamsComp::GetLightBarriersCount() const
{
	return int(m_barrierPositions.size());
}


double CLineParamsComp::GetLightBarrierPosition(int barrierIndex) const
{
	I_ASSERT(barrierIndex >= 0);
	I_ASSERT(barrierIndex < int(m_barrierPositions.size()));

	return m_barrierPositions[barrierIndex];
}


void CLineParamsComp::SetLightBarrierPosition(int barrierIndex, double position)
{
	I_ASSERT(barrierIndex >= 0);
	I_ASSERT(barrierIndex < int(m_barrierPositions.size()));

	if (position != m_barrierPositions[barrierIndex]){
		istd::CChangeNotifier notifier(this);

		m_barrierPositions[barrierIndex] = position;
	}
}


// reimplemented (iser::ISerializable)

bool CLineParamsComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	retVal = retVal && SerializeName(archive);

	static iser::CArchiveTag lightBarriersTag("LightBarriers", "List of light barrier positions");
	static iser::CArchiveTag barrierTag("Barrier", "Single light barrier position");

	int barriersCount = int(m_barrierPositions.size());
	retVal = retVal && archive.BeginMultiTag(lightBarriersTag, barrierTag, barriersCount);
	if (barriersCount != int(m_barrierPositions.size())){
		return false;
	}

	for (int barrierIndex = 0; barrierIndex < barriersCount; ++barrierIndex){
		retVal = retVal && archive.BeginTag(barrierTag);
		retVal = retVal && archive.Process(m_barrierPositions[barrierIndex]);
		retVal = retVal && archive.EndTag(barrierTag);
	}

	retVal = retVal && archive.EndTag(lightBarriersTag);

	static iser::CArchiveTag inspectionUnitsTag("InspectionUnits", "List of inspection units");
	static iser::CArchiveTag unitTag("Unit", "Single inspection unit");

	int unitsCount = GetInspectionUnitsCount();
	retVal = retVal && archive.BeginMultiTag(inspectionUnitsTag, unitTag, unitsCount);
	if (unitsCount != GetInspectionUnitsCount()){
		return false;
	}

	for (int unitIndex = 0; unitIndex < unitsCount; ++unitIndex){
		retVal = retVal && archive.BeginTag(unitTag);
		IInspectionUnitParams& params = GetInspectionUnitParams(unitIndex);
		retVal = retVal && params.Serialize(archive);
		retVal = retVal && archive.EndTag(unitTag);
	}

	retVal = retVal && archive.EndTag(inspectionUnitsTag);

	static iser::CArchiveTag ejectorsTag("Ejectors", "List of ejectors");
	static iser::CArchiveTag ejectorTag("Ejector", "Single ejector");

	int ejectorsCount = GetEjectorsCount();
	retVal = retVal && archive.BeginMultiTag(ejectorsTag, ejectorTag, ejectorsCount);
	if (ejectorsCount != GetEjectorsCount()){
		return false;
	}

	for (int ejectorIndex = 0; ejectorIndex < ejectorsCount; ++ejectorIndex){
		retVal = retVal && archive.BeginTag(ejectorTag);
		IEjectorParams& params = GetEjectorParams(ejectorIndex);
		retVal = retVal && params.Serialize(archive);
		retVal = retVal && archive.EndTag(ejectorTag);
	}

	retVal = retVal && archive.EndTag(ejectorsTag);

	static iser::CArchiveTag ticksPerDistanceUnitTag("TicksPerDistanceUnit", "Number of encoder ticks per distance logical unit");
	retVal = retVal && archive.BeginTag(ticksPerDistanceUnitTag);
	retVal = retVal && archive.Process(m_ticksPerDistanceUnit);
	retVal = retVal && archive.EndTag(ticksPerDistanceUnitTag);

	static iser::CArchiveTag minObjectSizeTag("MinObjectSize", "Minimal size of inspected object in logical units");
	retVal = retVal && archive.BeginTag(minObjectSizeTag);
	retVal = retVal && archive.Process(m_minObjectSize);
	retVal = retVal && archive.EndTag(minObjectSizeTag);

	static iser::CArchiveTag positionToleranceTag("PositionTolerance", "Tolerance of position for other light barriers");
	retVal = retVal && archive.BeginTag(positionToleranceTag);
	retVal = retVal && archive.Process(m_positionTolerance);
	retVal = retVal && archive.EndTag(positionToleranceTag);

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CLineParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetName(*m_nameAttrPtr);

	m_ticksPerDistanceUnit = *m_ticksPerDistanceUnitAttrPtr;
	m_minObjectSize = *m_minObjectsSizeAttrPtr;
	m_positionTolerance = *m_positionToleranceAttrPtr;

	int barriersCount = m_barrierPositionsAttrPtr.GetCount();
	m_barrierPositions.resize(barriersCount);

	for (int barrierIndex = 0; barrierIndex < barriersCount; ++barrierIndex){
		m_barrierPositions[barrierIndex] = m_barrierPositionsAttrPtr[barrierIndex];
	}
}


} // namespace icntl


