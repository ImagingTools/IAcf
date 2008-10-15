#include "icntl/CDriverControllerComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "ilolv/CMultiTracerCommands.h"

#include "icntl/ILineParams.h"
#include "icntl/IInspectionUnitParams.h"
#include "icntl/IEjectorParams.h"


namespace icntl
{


CDriverControllerComp::CDriverControllerComp()
:	m_workMode(WM_NONE)
{
}


// reimplemented (icntl::IAutomaticController)

int CDriverControllerComp::GetWorkMode() const
{
	return m_workMode;
}


bool CDriverControllerComp::SetWorkMode(int mode)
{
	if (mode != m_workMode){
		if (m_commandCallerCompPtr.IsValid()){
			ilolv::CMultiTracerCommands::SetMode command;

			switch (mode){
			case WM_MANUAL:
				command.mode = ilolv::CTracerCommands::TM_MANUAL;
				break;

			case WM_AUTOMATIC:
				command.mode = ilolv::CTracerCommands::TM_AUTOMATIC;
				break;

			default:
				return false;
			}

			int responseSize;
			if (!m_commandCallerCompPtr->CallCommand(
						ilolv::CMultiTracerCommands::SetMode::Id,
						&command, sizeof(command),
						NULL, 0,
						responseSize)){
				return false;
			}
		}

		istd::CChangeNotifier notifier(this);

		m_workMode = mode;
	}

	return true;
}


bool CDriverControllerComp::IsModeSupported(int mode) const
{
	return (mode == WM_MANUAL) || (mode == WM_AUTOMATIC);
}


// reimplemented (icomp::CComponentBase)

void CDriverControllerComp::OnComponentCreated()
{
	if (!m_multiLineParamsCompPtr.IsValid()){
		return;
	}

	if (m_multiLineModelCompPtr.IsValid()){
		m_multiLineModelCompPtr->AttachObserver(this);
	}

	int linesCount = m_multiLineParamsCompPtr->GetLinesCount();
	for (int lineIndex = 0; lineIndex < linesCount; ++lineIndex){
		ILineParams& lineParams = m_multiLineParamsCompPtr->GetLineParams(lineIndex);

		imod::IModel* lineModelPtr = dynamic_cast<imod::IModel*>(&lineParams);
		if (lineModelPtr != NULL){
			lineModelPtr->AttachObserver(this);
		}

		int unitsCount = lineParams.GetInspectionUnitsCount();
		for (int unitIndex = 0; unitIndex < unitsCount; ++unitIndex){
			IInspectionUnitParams& unitParams = lineParams.GetInspectionUnitParams(unitIndex);

			imod::IModel* unitModelPtr = dynamic_cast<imod::IModel*>(&unitParams);
			if (unitModelPtr != NULL){
				unitModelPtr->AttachObserver(this);
			}
		}

		int ejectorsCount = lineParams.GetEjectorsCount();
		for (int ejectorIndex = 0; ejectorIndex < ejectorsCount; ++ejectorIndex){
			IEjectorParams& ejectorParams = lineParams.GetEjectorParams(ejectorIndex);

			imod::IModel* ejectorModelPtr = dynamic_cast<imod::IModel*>(&ejectorParams);
			if (ejectorModelPtr != NULL){
				ejectorModelPtr->AttachObserver(this);
			}
		}
	}

	SetParamsToDriver();
}


void CDriverControllerComp::OnComponentDestroyed()
{
	EnsureDetached();
}


// protected methods

bool CDriverControllerComp::SetParamsToDriver()
{
	I_ASSERT(m_commandCallerCompPtr.IsValid());
	I_ASSERT(m_workMode != WM_AUTOMATIC);

	int responseSize;

	int linesCount = m_multiLineParamsCompPtr->GetLinesCount();
	ilolv::CMultiTracerCommands::SetParams setParamsCommand;
	setParamsCommand.linesCount = linesCount;

	if (!m_commandCallerCompPtr->CallCommand(
				ilolv::CMultiTracerCommands::SetParams::Id,
				&setParamsCommand, sizeof(setParamsCommand),
				NULL, 0,
				responseSize)){
		return false;
	}

	for (int lineIndex = 0; lineIndex < linesCount; ++lineIndex){
		const ILineParams& lineParams = m_multiLineParamsCompPtr->GetLineParams(lineIndex);

		if (!SetLineParamsToDriver(lineIndex, lineParams)){
			return false;
		}
	}

	return true;
}


bool CDriverControllerComp::SetLineParamsToDriver(int lineIndex, const ILineParams& lineParams)
{
	I_ASSERT(lineIndex >= 0);
	I_ASSERT(m_commandCallerCompPtr.IsValid());
	I_ASSERT(m_workMode != WM_AUTOMATIC);

	int unitsCount = lineParams.GetInspectionUnitsCount();
	int ejectorsCount = lineParams.GetEjectorsCount();
	int lightBarriersCount = lineParams.GetLightBarriersCount();
	double ticksPerDistUnit = lineParams.GetTicksPerDistanceUnit();

	int responseSize = 0;

	ilolv::CMultiTracerCommands::SetLineParams setLineParamsCommand;
	setLineParamsCommand.lineIndex = lineIndex;
	setLineParamsCommand.line.unitsCount = unitsCount;
	setLineParamsCommand.line.ejectorsCount = ejectorsCount;
	setLineParamsCommand.line.lightBarriersCount = lightBarriersCount;
	setLineParamsCommand.line.autonomeEjectorIndex = (lineIndex < m_autonomeEjectorIndicesAttrPtr.GetCount())?
				m_autonomeEjectorIndicesAttrPtr[lineIndex]:
				0;
	setLineParamsCommand.line.minObjectSize = int(lineParams.GetMinObjectSize() * ticksPerDistUnit + 0.5);
	setLineParamsCommand.line.positionTolerance = int(lineParams.GetPositionTolerance() * ticksPerDistUnit + 0.5);
	setLineParamsCommand.line.ecLightBarrierIndex = (lineIndex < m_ejectionControlIndicesAttrPtr.GetCount())?
				m_ejectionControlIndicesAttrPtr[lineIndex]:
				-1;
	if (!m_commandCallerCompPtr->CallCommand(
				ilolv::CMultiTracerCommands::SetLineParams::Id,
				&setLineParamsCommand, sizeof(setLineParamsCommand),
				NULL, 0,
				responseSize)){
		return false;
	}

	ilolv::CMultiTracerCommands::SetLineIoParams setLineIoParamsCommand;
	setLineIoParamsCommand.lineIndex = lineIndex;
	setLineIoParamsCommand.line.lightBarriersBitIndex = (lineIndex < m_lightBarriersBitsAttrPtr.GetCount())?
				m_lightBarriersBitsAttrPtr[lineIndex]:
				-1;
	setLineIoParamsCommand.line.ejectorsBitIndex = (lineIndex < m_ejectorsBitsAttrPtr.GetCount())?
				m_ejectorsBitsAttrPtr[lineIndex]:
				-1;
	setLineIoParamsCommand.line.triggersBitIndex = (lineIndex < m_triggersBitsAttrPtr.GetCount())?
				m_triggersBitsAttrPtr[lineIndex]:
				-1;
	if (!m_commandCallerCompPtr->CallCommand(
				ilolv::CMultiTracerCommands::SetLineIoParams::Id,
				&setLineIoParamsCommand, sizeof(setLineIoParamsCommand),
				NULL, 0,
				responseSize)){
		return false;
	}

	for (int barrierIndex = 0; barrierIndex < lightBarriersCount; ++barrierIndex){
		ilolv::CMultiTracerCommands::SetLightBarrierParams setBarrierParamsCommand;
		setBarrierParamsCommand.lineIndex = lineIndex;
		setBarrierParamsCommand.line.barrierIndex = barrierIndex;
		setBarrierParamsCommand.line.offset = int(lineParams.GetLightBarrierPosition(barrierIndex) * ticksPerDistUnit + 0.5);
		if (!m_commandCallerCompPtr->CallCommand(
					ilolv::CMultiTracerCommands::SetLightBarrierParams::Id,
					&setBarrierParamsCommand, sizeof(setBarrierParamsCommand),
					NULL, 0,
					responseSize)){
			return false;
		}
	}

	for (int unitIndex = 0; unitIndex < unitsCount; ++unitIndex){
		const IInspectionUnitParams& unitParams = lineParams.GetInspectionUnitParams(unitIndex);

		if (!SetUnitParamsToDriver(lineIndex, unitIndex, unitParams, ticksPerDistUnit)){
			return false;
		}
	}

	for (int ejectorIndex = 0; ejectorIndex < ejectorsCount; ++ejectorIndex){
		const IEjectorParams& ejectorParams = lineParams.GetEjectorParams(ejectorIndex);

		if (!SetEjectorParamsToDriver(lineIndex, ejectorIndex, ejectorParams, ticksPerDistUnit)){
			return false;
		}
	}

	return true;
}


bool CDriverControllerComp::SetUnitParamsToDriver(
			int lineIndex,
			int unitIndex,
			const IInspectionUnitParams& unitParams,
			double ticksPerDistUnit)
{
	I_ASSERT(lineIndex >= 0);
	I_ASSERT(unitIndex >= 0);
	I_ASSERT(m_commandCallerCompPtr.IsValid());
	I_ASSERT(m_workMode != WM_AUTOMATIC);

	ilolv::CMultiTracerCommands::SetUnitParams command;
	command.lineIndex = lineIndex;
	command.line.unitIndex = unitIndex;
	command.line.unit.lightBarrierIndex = unitParams.IsEnabled()? unitIndex: -1;
	command.line.unit.edgePosition = int(unitParams.GetTriggerAlpha() * ilolv::CInspectionUnitCommands::UnitParams::FALLING_EDGE);
	command.line.unit.triggerDuration = int(*m_triggerDurationAttrPtr * 1e6);
	command.line.unit.triggerRelaxationTime = int(*m_triggerRelaxationAttrPtr * 1e6);
	command.line.unit.triggerOffset = int(unitParams.GetTriggerOffset() * ticksPerDistUnit + 0.5);

	int responseSize = 0;
	return m_commandCallerCompPtr->CallCommand(
				ilolv::CMultiTracerCommands::SetUnitParams::Id,
				&command, sizeof(command),
				NULL, 0,
				responseSize);
}


bool CDriverControllerComp::SetEjectorParamsToDriver(
			int lineIndex,
			int ejectorIndex,
			const IEjectorParams& ejectorParams,
			double ticksPerDistUnit)
{
	I_ASSERT(lineIndex >= 0);
	I_ASSERT(ejectorIndex >= 0);
	I_ASSERT(m_commandCallerCompPtr.IsValid());
	I_ASSERT(m_workMode != WM_AUTOMATIC);

	ilolv::CMultiTracerCommands::SetEjectorParams command;
	command.lineIndex = lineIndex;
	command.line.ejectorIndex = ejectorIndex;
	command.line.ejector.isEnabled = ejectorParams.IsEnabled();
	command.line.ejector.maxEjectorOnTime = int(*m_maxEjectorOnTimeAttrPtr * 1e6);
	command.line.ejector.position = int(ejectorParams.GetEjectorPosition() * ticksPerDistUnit + 0.5);
	command.line.ejector.onDistance = int(ejectorParams.GetEjectionDistance() * ticksPerDistUnit + 0.5);
	command.line.ejector.reactionDelay = 0;

	int responseSize = 0;
	return m_commandCallerCompPtr->CallCommand(
				ilolv::CMultiTracerCommands::SetEjectorParams::Id,
				&command, sizeof(command),
				NULL, 0,
				responseSize);
}


// reimplemented (imod::CMultiModelObserverBase)

void CDriverControllerComp::OnUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (m_commandCallerCompPtr.IsValid() && (m_workMode != WM_AUTOMATIC)){
		SetParamsToDriver();
	}
}


} // namespace icntl


