#include "iqtcntl/CDriverControllerComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "ilolv/CSignalBitsCommands.h"
#include "icntl/ILineParams.h"
#include "icntl/IInspectionUnitParams.h"
#include "icntl/IEjectorParams.h"


namespace iqtcntl
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


// reimplemented (icntl::IMultiLineController)

int CDriverControllerComp::GetLinesCount() const
{
	return int(m_lineControllers.size());
}


icntl::ILineController& CDriverControllerComp::GetLineController(int lineIndex) const
{
	return m_lineControllers[lineIndex];
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
	m_lineControllers.resize(linesCount);
	for (int lineIndex = 0; lineIndex < linesCount; ++lineIndex){
		icntl::ILineParams& lineParams = m_multiLineParamsCompPtr->GetLineParams(lineIndex);

		imod::IModel* lineModelPtr = dynamic_cast<imod::IModel*>(&lineParams);
		if (lineModelPtr != NULL){
			lineModelPtr->AttachObserver(this);
		}

		int unitsCount = lineParams.GetInspectionUnitsCount();
		for (int unitIndex = 0; unitIndex < unitsCount; ++unitIndex){
			icntl::IInspectionUnitParams& unitParams = lineParams.GetInspectionUnitParams(unitIndex);

			imod::IModel* unitModelPtr = dynamic_cast<imod::IModel*>(&unitParams);
			if (unitModelPtr != NULL){
				unitModelPtr->AttachObserver(this);
			}
		}

		int ejectorsCount = lineParams.GetEjectorsCount();
		for (int ejectorIndex = 0; ejectorIndex < ejectorsCount; ++ejectorIndex){
			icntl::IEjectorParams& ejectorParams = lineParams.GetEjectorParams(ejectorIndex);

			imod::IModel* ejectorModelPtr = dynamic_cast<imod::IModel*>(&ejectorParams);
			if (ejectorModelPtr != NULL){
				ejectorModelPtr->AttachObserver(this);
			}
		}
	}

	if (m_commandCallerCompPtr.IsValid()){
		int signalBitsCount = m_signalBitsAttrPtr.GetCount();

		ilolv::CSignalBitsCommands::SetParams setSignalParamsCommand;
		setSignalParamsCommand.heartbeatPeriod = I_DWORD(*m_heartbeatPeriodAttrPtr * 1e6);
		setSignalParamsCommand.signalBitsCount = signalBitsCount;

		int responseSize;
		m_commandCallerCompPtr->CallCommand(
					ilolv::CSignalBitsCommands::SetParams::Id,
					&setSignalParamsCommand, sizeof(setSignalParamsCommand),
					NULL, 0,
					responseSize);

		for (int signalIndex = 0; signalIndex < signalBitsCount; ++signalIndex){
			if (m_signalBitsAttrPtr[signalIndex] >= 0){
				ilolv::CSignalBitsCommands::SetSignalBitIndex setSignalBitIndexCommand;
				setSignalBitIndexCommand.signalIndex = signalIndex;
				setSignalBitIndexCommand.bitIndex = m_signalBitsAttrPtr[signalIndex];

				m_commandCallerCompPtr->CallCommand(
							ilolv::CSignalBitsCommands::SetSignalBitIndex::Id,
							&setSignalBitIndexCommand, sizeof(setSignalBitIndexCommand),
							NULL, 0,
							responseSize);
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

	m_lineControllers.resize(linesCount);

	for (int lineIndex = 0; lineIndex < linesCount; ++lineIndex){
		const icntl::ILineParams& lineParams = m_multiLineParamsCompPtr->GetLineParams(lineIndex);
		LineController& lineController = m_lineControllers[lineIndex];

		if (!SetLineParamsToDriver(lineIndex, lineParams, lineController)){
			return false;
		}
	}

	return true;
}


bool CDriverControllerComp::SetLineParamsToDriver(
			int lineIndex,
			const icntl::ILineParams& lineParams,
			LineController& controller)
{
	I_ASSERT(lineIndex >= 0);
	I_ASSERT(m_commandCallerCompPtr.IsValid());
	I_ASSERT(m_workMode != WM_AUTOMATIC);

	int unitsCount = lineParams.GetInspectionUnitsCount();
	int ejectorsCount = lineParams.GetEjectorsCount();
	int lightBarriersCount = lineParams.GetLightBarriersCount();
	double ticksPerDistUnit = lineParams.GetTicksPerDistanceUnit();

	controller.lineIndex = lineIndex;
	controller.parentPtr = this;
	controller.ticksPerDistanceUnit = ticksPerDistUnit;

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

	controller.unitControllers.resize(unitsCount);
	for (int unitIndex = 0; unitIndex < unitsCount; ++unitIndex){
		const icntl::IInspectionUnitParams& unitParams = lineParams.GetInspectionUnitParams(unitIndex);
		UnitController& unitController = controller.unitControllers[unitIndex];

		if (!SetUnitParamsToDriver(lineIndex, unitIndex, unitParams, ticksPerDistUnit, unitController)){
			return false;
		}
	}

	controller.ejectorControllers.resize(ejectorsCount);
	for (int ejectorIndex = 0; ejectorIndex < ejectorsCount; ++ejectorIndex){
		const icntl::IEjectorParams& ejectorParams = lineParams.GetEjectorParams(ejectorIndex);
		EjectorController& ejectorController = controller.ejectorControllers[ejectorIndex];

		if (!SetEjectorParamsToDriver(lineIndex, ejectorIndex, ejectorParams, ticksPerDistUnit, ejectorController)){
			return false;
		}
	}

	return true;
}


bool CDriverControllerComp::SetUnitParamsToDriver(
			int lineIndex,
			int unitIndex,
			const icntl::IInspectionUnitParams& unitParams,
			double ticksPerDistUnit,
			UnitController& controller)
{
	I_ASSERT(lineIndex >= 0);
	I_ASSERT(unitIndex >= 0);
	I_ASSERT(m_commandCallerCompPtr.IsValid());
	I_ASSERT(m_workMode != WM_AUTOMATIC);

	controller.lineIndex = lineIndex;
	controller.unitIndex = unitIndex;
	controller.parentPtr = this;

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
			const icntl::IEjectorParams& ejectorParams,
			double ticksPerDistUnit,
			EjectorController& controller)
{
	I_ASSERT(lineIndex >= 0);
	I_ASSERT(ejectorIndex >= 0);
	I_ASSERT(m_commandCallerCompPtr.IsValid());
	I_ASSERT(m_workMode != WM_AUTOMATIC);

	controller.lineIndex = lineIndex;
	controller.ejectorIndex = ejectorIndex;
	controller.parentPtr = this;

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


// public methods of private embedded class ObjectInspection

CDriverControllerComp::ObjectInspection::ObjectInspection(
			ilolv::ICommandCaller* commandCallerPtr,
			int lineIndex,
			int stationIndex,
			const ilolv::CMultiTracerCommands::PopId::Result& objectParams)
:	m_commandCaller(*commandCallerPtr),
	m_lineIndex(lineIndex),
	m_unitIndex(stationIndex),
	m_objectId(objectParams.objectIndex),
	m_inspectionId(objectParams.inspectionId)
{
	I_ASSERT(commandCallerPtr != NULL);

	m_timestamp.SetNativeRepresentation(objectParams.nativeTimestamp);
}


// reimplemented (icntl::IObjectInspection)

I_DWORD CDriverControllerComp::ObjectInspection::GetObjectId() const
{
	return m_objectId;
}


const isys::ITimer& CDriverControllerComp::ObjectInspection::GetTimestamp() const
{
	return m_timestamp;
}


bool CDriverControllerComp::ObjectInspection::SetEjector(int ejectorIndex)
{
	ilolv::CMultiTracerCommands::SetResult command;
	command.lineIndex = m_lineIndex;
	command.line.unitIndex = m_unitIndex;
	command.line.unit.inspectionId = m_inspectionId;
	command.line.unit.ejectorIndex = ejectorIndex;

	ilolv::CMultiTracerCommands::SetResult::Result commandResult;

	int resultSize;
	if (m_commandCaller.CallCommand(
				ilolv::CMultiTracerCommands::SetResult::Id,
				&command, sizeof(command),
				&commandResult, sizeof(commandResult),
				resultSize) && (resultSize >= sizeof(commandResult))){
		return commandResult.wasSet;
	}

	return false;
}


// public methods of private embedded class UnitController

// reimplemented (icntl::ILineController)

icntl::IObjectInspection* CDriverControllerComp::UnitController::PopObjectInspection()
{
	I_ASSERT(parentPtr != NULL);

	if (parentPtr->m_commandCallerCompPtr.IsValid()){
		ilolv::CMultiTracerCommands::PopId command;
		command.lineIndex = lineIndex;
		command.line.unitIndex = unitIndex;

		ilolv::CMultiTracerCommands::PopId::Result commandResult;

		int resultSize;
		if (parentPtr->m_commandCallerCompPtr->CallCommand(
					ilolv::CMultiTracerCommands::PopId::Id,
					&command, sizeof(command),
					&commandResult, sizeof(commandResult),
					resultSize)){
			if  ((resultSize >= sizeof(commandResult)) && (commandResult.inspectionId >= 0)){
				return new ObjectInspection(
							parentPtr->m_commandCallerCompPtr.GetPtr(),
							lineIndex,
							unitIndex,
							commandResult);
			}
		}
	}

	return NULL;
}


isys::ITimer* CDriverControllerComp::UnitController::DoCameraTrigger()
{
	I_ASSERT(parentPtr != NULL);

	if (parentPtr->m_commandCallerCompPtr.IsValid()){
		ilolv::CMultiTracerCommands::SingleTrigger command;
		command.lineIndex = lineIndex;
		command.line.unitIndex = unitIndex;

		ilolv::CMultiTracerCommands::SingleTrigger::Result commandResult;

		int resultSize;
		if (parentPtr->m_commandCallerCompPtr->CallCommand(
					ilolv::CMultiTracerCommands::SingleTrigger::Id,
					&command, sizeof(command),
					&commandResult, sizeof(commandResult),
					resultSize) && (resultSize >= sizeof(commandResult))){
			iwin::CTimer* timerPtr = new iwin::CTimer;
			if (timerPtr != NULL){
				timerPtr->SetNativeRepresentation(commandResult.nativeTimestamp);

				return timerPtr;
			}
		}
	}

	return NULL;
}


// public methods of private embedded class EjectorController

// reimplemented (icntl::IEjectorController)

int CDriverControllerComp::EjectorController::GetEjectedCounter() const
{
	return 0;
}


bool CDriverControllerComp::EjectorController::DoTestEjection()
{
	return false;
}


// public methods of private embedded class LineController

// reimplemented (icntl::ILineController)

int CDriverControllerComp::LineController::GetInspectionUnitsCount() const
{
	return int(unitControllers.size());
}


icntl::IInspectionUnitController& CDriverControllerComp::LineController::GetInspectionUnitController(int unitIndex) const
{
	I_ASSERT(unitIndex >= 0);
	I_ASSERT(unitIndex < int(unitControllers.size()));

	return unitControllers[unitIndex];
}


int CDriverControllerComp::LineController::GetEjectorsCount() const
{
	return int(ejectorControllers.size());
}


icntl::IEjectorController& CDriverControllerComp::LineController::GetEjectorController(int ejectorIndex) const
{
	I_ASSERT(ejectorIndex >= 0);
	I_ASSERT(ejectorIndex < int(ejectorControllers.size()));

	return ejectorControllers[ejectorIndex];
}


bool CDriverControllerComp::LineController::GetTransmissionPosition(double& result) const
{
	I_ASSERT(parentPtr != NULL);

	if (parentPtr->m_commandCallerCompPtr.IsValid()){
		ilolv::CMultiTracerCommands::GetLineInfo command;
		command.lineIndex = lineIndex;

		ilolv::CMultiTracerCommands::GetLineInfo::Result commandResult;

		int resultSize;
		if (parentPtr->m_commandCallerCompPtr->CallCommand(
					ilolv::CMultiTracerCommands::GetLineInfo::Id,
					&command, sizeof(command),
					&commandResult, sizeof(commandResult),
					resultSize) && (resultSize >= sizeof(commandResult))){

			result = ticksPerDistanceUnit * commandResult.linePos;

			return true;
		}
	}

	return false;
}


bool CDriverControllerComp::LineController::GetObjectPosition(I_DWORD /*objectId*/, double& /*result*/) const
{
	return false;
}


bool CDriverControllerComp::LineController::GetLastObjectId(I_DWORD& result) const
{
	I_ASSERT(parentPtr != NULL);

	if (parentPtr->m_commandCallerCompPtr.IsValid()){
		ilolv::CMultiTracerCommands::GetLineInfo command;
		command.lineIndex = lineIndex;

		ilolv::CMultiTracerCommands::GetLineInfo::Result commandResult;

		int resultSize;
		if (parentPtr->m_commandCallerCompPtr->CallCommand(
					ilolv::CMultiTracerCommands::GetLineInfo::Id,
					&command, sizeof(command),
					&commandResult, sizeof(commandResult),
					resultSize) && (resultSize >= sizeof(commandResult))){

			result = commandResult.lastDetectedObjectIndex;

			return true;
		}
	}

	return false;
}


} // namespace iqtcntl


