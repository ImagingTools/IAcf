#include "ilolv/CTracerDriverBase.h"


#include "ilolv/CGeneralInfoCommands.h"


namespace ilolv
{


CTracerDriverBase::CTracerDriverBase()
:	m_queueEndPosition(-1)
{
	m_params.unitsCount = 0;
	m_params.ejectorsCount = 0;
	m_params.autonomeEjectorIndex = -1;
	m_params.minObjectSize = 200;
	m_params.minObjectsDistance = 300;
	m_params.positionTolerance = 10;
	m_params.ecLightBarrierIndex = -1;

	for (int unitIndex = 0; unitIndex < MAX_UNITS_COUNT; ++unitIndex){
		InspectionUnitElement& unit = m_inspectionUnits[unitIndex];

		unit.lightBarrierIndex = -1;
		unit.triggerOffset = 0;
		unit.triggerDuration = 10000;
		unit.triggerRelaxationTime = 20000;

		unit.triggerOnTime = 0;
		unit.isTriggerBitSet = false;
		unit.edgeOnPosition = 0;
		unit.lastBarrierState = true;
	}

	for (int ejectorIndex = 0; ejectorIndex < MAX_EJECTORS; ++ejectorIndex){
		EjectorInfo& ejector = m_ejectors[ejectorIndex];

		ejector.isEnabled = false;
		ejector.position = 0;
		ejector.onDistance = 200;
		ejector.maxEjectorOnTime = 10000;

		ejector.overloadCounter = 0;
		ejector.ejectionOnTime = 0;
	}

	for (int barrierIndex = 0; barrierIndex < MAX_LIGHT_BARRIERS; ++barrierIndex){
		m_lightBarriersOffset[barrierIndex] = 1000;
	}

	m_lastInspectedObjectIndex = 0;

	m_controllerMode = CTracerCommands::TM_DISABLED;

	m_ejectorOnEventIndex = EI_TRIGGER;
	m_ejectorOffEventIndex = EI_TRIGGER;
	m_firstStationIndex = 0;
}


void CTracerDriverBase::ResetQueue()
{
	m_objectsFifo.Reset();

	I_DWORD linePosition = GetLinePosition();

	int maxPopFifoDistance = 0;
	for (int unitIndex = 0; unitIndex < m_params.unitsCount; ++unitIndex){
		InspectionUnitElement& unit = m_inspectionUnits[unitIndex];

		unit.triggerOnTime = 0;
		unit.isTriggerBitSet = 0;
		unit.lastBarrierState = true;

		if (unit.lightBarrierIndex >= 0){
			int lightBarrierOffset = m_lightBarriersOffset[unit.lightBarrierIndex] + unit.triggerOffset;

			unit.edgeOnPosition =
							linePosition +
							lightBarrierOffset -
							m_params.minObjectsDistance;

			if (lightBarrierOffset >= maxPopFifoDistance){
				maxPopFifoDistance = lightBarrierOffset + 1;
			}
		}
		else{
			unit.edgeOnPosition = 0;
		}

		SetTriggerBit(unitIndex, false);
	}

	int minEjectorOnDistance = -1;
	for (int ejectorIndex = 0; ejectorIndex < m_params.ejectorsCount; ++ejectorIndex){
		EjectorInfo& ejector = m_ejectors[ejectorIndex];

		ejector.overloadCounter = 0;
		ejector.ejectionOnTime = 0;

		if (ejector.isEnabled && (ejector.position >= 0)){
			if ((minEjectorOnDistance < 0) || (ejector.position < minEjectorOnDistance)){
				minEjectorOnDistance = ejector.position;
			}

			int ejectorOffDistance = ejector.position + ejector.onDistance;
			if (ejectorOffDistance > maxPopFifoDistance){
				maxPopFifoDistance = ejectorOffDistance;
			}
		}

		SetEjectorBit(ejectorIndex, false);
	}

	if (m_params.ecLightBarrierIndex >= 0){
		int lightBarrierOffset = m_lightBarriersOffset[m_params.ecLightBarrierIndex];
		if (lightBarrierOffset > maxPopFifoDistance){
			maxPopFifoDistance = lightBarrierOffset;
		}
	}

	m_ejectorOnEventIndex = EI_TRIGGER + m_params.unitsCount;
	m_ejectorOffEventIndex = m_ejectorOnEventIndex + m_params.ejectorsCount;

	m_decisionEventPosition = minEjectorOnDistance;
	m_queueEndPosition = maxPopFifoDistance;

	if (!SetCounterQueuesCount(m_ejectorOffEventIndex + m_params.ejectorsCount)){
		AppendMessage(CGeneralInfoCommands::MC_CRITICAL,
					CGeneralInfoCommands::MI_INTERNAL_ERROR,
					"More queues needed than supported",
					true);
	}
}


const CInspectionUnitCommands::UnitParams& CTracerDriverBase::GetUnitParams(int unitIndex) const
{
	I_ASSERT(unitIndex >= 0);
	I_ASSERT(unitIndex < m_params.unitsCount);

	return m_inspectionUnits[unitIndex];
}


const CTracerCommands::EjectorParams& CTracerDriverBase::GetEjectorParams(int ejectorIndex) const
{
	I_ASSERT(ejectorIndex >= 0);
	I_ASSERT(ejectorIndex < m_params.ejectorsCount);
	I_ASSERT(m_params.ejectorsCount <= MAX_EJECTORS);

	return m_ejectors[ejectorIndex];
}


int CTracerDriverBase::OnPopInspectionCommand(int unitIndex)
{
	if ((m_controllerMode == CTracerCommands::TM_AUTOMATIC) && (unitIndex < m_params.unitsCount)){
		InspectionUnitElement& unit = m_inspectionUnits[unitIndex];

		if (unit.lightBarrierIndex >= 0){
			for (		int inspectionId = m_objectsFifo.GetBackIndex();
						inspectionId != m_objectsFifo.GetNextFrontIndex();
						inspectionId = m_objectsFifo.GetNextIndex(inspectionId)){
				ObjectInfo& objectInfo = m_objectsFifo.GetObjectAt(inspectionId);
				InspectionInfo& inspectionInfo = objectInfo.units[unitIndex];

				InspectionState state = inspectionInfo.inspectionState;
				if (state <= IS_INIT){
					break;
				}

				if (state == IS_TRIGGERED){
					inspectionInfo.inspectionState = IS_INSPECTION;

					return inspectionId;
				}
			}
		}
	}

	return -1;
}


bool CTracerDriverBase::OnSetResultCommand(int unitIndex, int inspectionId, int ejectorIndex)
{
	I_ASSERT(inspectionId >= 0);
	I_ASSERT(inspectionId < FIFO_COUNT);
	I_ASSERT(ejectorIndex < m_params.ejectorsCount);

	if ((inspectionId >= 0) && (inspectionId < FIFO_COUNT)){
		ObjectInfo& objectInfo = m_objectsFifo.GetObjectAt(inspectionId);
		if (objectInfo.objectState >= OS_EJECTION_DECIDED){		// ejection decision was made, it is too late
			return false;
		}

		InspectionInfo& inspectionInfo = objectInfo.units[unitIndex];
		inspectionInfo.inspectionState = IS_RESULT;
		inspectionInfo.ejectorIndex = ejectorIndex;
	}

	return true;
}


void CTracerDriverBase::ProcessPositionEvent(int eventIndex, void* userContext)
{
	if (eventIndex < EI_TRIGGER){
		switch (eventIndex){
		case EI_DECISION:
			OnDecisionEvent(int(userContext));
			break;

		case EI_EJECTION_CONTROL:
			OnEjectionControlEvent(int(userContext));
			break;

		case EI_REMOVE_FROM_QUEUE:
			OnPopFifoEvent();
			break;
		}
	}
	else{
		if (eventIndex < m_ejectorOnEventIndex){
			// trigger event, called at position where object should be triggered
			int unitIndex = eventIndex - EI_TRIGGER;
			I_ASSERT(unitIndex < m_params.unitsCount);

			OnTriggerEvent(unitIndex, int(userContext));
		}
		else if (eventIndex < m_ejectorOffEventIndex){
			// "ejector on" event, called at position where ejector should be turned on
			int ejectorIndex = eventIndex - m_ejectorOnEventIndex;
			I_ASSERT(ejectorIndex < m_params.ejectorsCount);

			OnEjectorOnEvent(ejectorIndex);
		}
		else if (eventIndex < m_ejectorOffEventIndex + m_params.ejectorsCount){
			// "ejector off" event, called at position where ejector should be turned off
			int ejectorIndex = eventIndex - m_ejectorOffEventIndex;
			I_ASSERT(ejectorIndex < m_params.ejectorsCount);

			OnEjectorOffEvent(ejectorIndex);
		}
	}
}


// reimplemented (ilolv::IDriver)

bool CTracerDriverBase::OnCommand(
			I_DWORD commandCode,
			const void* commandBuffer,
			int commandBufferSize,
			void* responseBuffer,
			int responseBufferSize,
			I_DWORD& responseSize)
{
	responseSize = 0;

	switch (commandCode){
	case CTracerCommands::SetParams::Id:
		if (commandBufferSize >= sizeof(CTracerCommands::SetParams)){
			m_params = *(const CTracerCommands::SetParams*)commandBuffer;

			if (m_params.unitsCount > MAX_UNITS_COUNT){
				m_params.unitsCount = MAX_UNITS_COUNT;
			}

			if (m_params.ejectorsCount > MAX_EJECTORS){
				m_params.ejectorsCount = MAX_EJECTORS;
			}

			if (m_params.lightBarriersCount > MAX_LIGHT_BARRIERS){
				m_params.lightBarriersCount = MAX_LIGHT_BARRIERS;
			}

			if (m_params.autonomeEjectorIndex >= m_params.ejectorsCount){
				m_params.autonomeEjectorIndex = m_params.ejectorsCount - 1;
			}

			if (m_params.ecLightBarrierIndex >= m_params.lightBarriersCount){
				m_params.ecLightBarrierIndex = -1;
			}
		}
		break;

	case CTracerCommands::SetUnitParams::Id:
		if (commandBufferSize >= sizeof(CTracerCommands::SetUnitParams)){
			const CTracerCommands::SetUnitParams& command = *(const CTracerCommands::SetUnitParams*)commandBuffer;

			if ((command.unitIndex >= 0) && (command.unitIndex < m_params.unitsCount)){
				CInspectionUnitCommands::UnitParams& params = m_inspectionUnits[command.unitIndex];

				params = command.unit;

				if (params.lightBarrierIndex >= m_params.lightBarriersCount){
					params.lightBarrierIndex = -1;
				}
			}
		}
		break;

	case CTracerCommands::SetEjectorParams::Id:
		if (commandBufferSize >= sizeof(CTracerCommands::SetEjectorParams)){
			const CTracerCommands::SetEjectorParams& command = *(const CTracerCommands::SetEjectorParams*)commandBuffer;

			int ejectorIndex = int(command.ejectorIndex);
			if ((ejectorIndex >= 0) && (ejectorIndex < m_params.ejectorsCount)){
				CTracerCommands::EjectorParams& params = m_ejectors[ejectorIndex];

				params = command.ejector;
			}
		}
		break;

	case CTracerCommands::SetLightBarrierParams::Id:
		if (commandBufferSize >= sizeof(CTracerCommands::SetLightBarrierParams)){
			const CTracerCommands::SetLightBarrierParams& command = *(const CTracerCommands::SetLightBarrierParams*)commandBuffer;

			int lightbarrierIndex = int(command.barrierIndex);
			if ((lightbarrierIndex >= 0) && (lightbarrierIndex < m_params.lightBarriersCount)){
				m_lightBarriersOffset[lightbarrierIndex] = command.offset;
			}
		}
		break;

	case CTracerCommands::SetMode::Id:
		if (commandBufferSize >= sizeof(CTracerCommands::SetMode)){
			const CTracerCommands::SetMode& command = *(const CTracerCommands::SetMode*)commandBuffer;

			m_controllerMode = command.mode;

			ResetQueue();
		}
		break;

	case CTracerCommands::SingleTrigger::Id:
		if (		(commandBufferSize >= sizeof(CTracerCommands::SingleTrigger)) &&
					(responseBufferSize >= sizeof(CTracerCommands::SingleTrigger::Result))){
			OnSingleTriggerCommand(
						*(const CTracerCommands::SingleTrigger*)commandBuffer,
						*(CTracerCommands::SingleTrigger::Result*)responseBuffer);
			responseSize = sizeof(CTracerCommands::SingleTrigger::Result);
		}
		break;

	case CTracerCommands::GetLineInfo::Id:
		if (responseBufferSize >= sizeof(CTracerCommands::GetLineInfo::Result)){
			CTracerCommands::GetLineInfo::Result& result = *(CTracerCommands::GetLineInfo::Result*)responseBuffer;

			result.linePos = GetLinePosition();
			result.lastDetectedObjectIndex = m_lastInspectedObjectIndex;

			responseSize = sizeof(CTracerCommands::GetLineInfo::Result);
		}
		break;

	case CTracerCommands::PopId::Id:
		if (		(commandBufferSize >= sizeof(CTracerCommands::PopId)) &&
					(responseBufferSize >= sizeof(CTracerCommands::PopId::Result))){
			const CTracerCommands::PopId& command = *(const CTracerCommands::PopId*)commandBuffer;
			CTracerCommands::PopId::Result& result = *(CTracerCommands::PopId::Result*)responseBuffer;

			int inspectionId = OnPopInspectionCommand(command.unitIndex);

			result.inspectionId = inspectionId;
			if (inspectionId >= 0){
				const ObjectInfo& objectInfo = m_objectsFifo.GetObjectAt(inspectionId);

				result.nativeTimestamp = objectInfo.units[command.unitIndex].nativeTimeStamp;
				result.objectIndex = objectInfo.objectIndex;
				result.objectPosition = m_objectsFifo.GetPositionAt(inspectionId);
			}

			responseSize = sizeof(CTracerCommands::PopId::Result);
		}
		break;

	case CTracerCommands::SetResult::Id:
		if (		(commandBufferSize >= sizeof(CTracerCommands::SetResult)) &&
					(responseBufferSize >= sizeof(CTracerCommands::SetResult::Result))){
			const CTracerCommands::SetResult& command = *(const CTracerCommands::SetResult*)commandBuffer;
			CTracerCommands::SetResult::Result& result = *(CTracerCommands::SetResult::Result*)responseBuffer;

			if ((command.unitIndex >= 0) && (command.unitIndex < m_params.unitsCount)){
				int ejectorIndex;
				if (command.unit.result != 0){
					ejectorIndex = -1;
				}
				else{
					ejectorIndex = command.unit.ejectorIndex;
				}

				result.wasSet = OnSetResultCommand(
								command.unitIndex,
								command.unit.inspectionId,
								ejectorIndex);

				responseSize = sizeof(CTracerCommands::SetResult::Result);
			}
		}
		break;

	default:
		return false;
	}

	return true;
}


void CTracerDriverBase::OnHardwareInterrupt(I_DWORD interruptFlags)
{
	if ((interruptFlags & IF_ENCODER_INTERRUPT) != 0){
		I_DWORD linePosition = GetLinePosition();

		for (int i = 0; i < m_params.unitsCount; ++i){
			InspectionUnitElement& unit = m_inspectionUnits[i];
			int lightBarrierIndex = unit.lightBarrierIndex;
			if (lightBarrierIndex >= 0){
				bool lightBarrierBit = GetLightBarrierBit(lightBarrierIndex);
				if (lightBarrierBit && !unit.lastBarrierState){
					if (unit.edgePosition == 0){
						if (I_SDWORD(linePosition - unit.edgeOnPosition - m_params.minObjectsDistance) > 0){
							int lightBarrierOffset = m_lightBarriersOffset[lightBarrierIndex];

							OnLightBarrierEdge(linePosition - lightBarrierOffset, i);
						}
					}

					unit.edgeOnPosition = linePosition;
				}
				else if (!lightBarrierBit && unit.lastBarrierState){
					if (unit.edgePosition != 0){
						I_DWORD objectSize = linePosition - unit.edgeOnPosition;
						if (int(objectSize) < m_params.minObjectSize){
							int objectOffset = objectSize * unit.edgePosition / CInspectionUnitCommands::UnitParams::FALLING_EDGE;
							int lightBarrierOffset = m_lightBarriersOffset[lightBarrierIndex];

							OnLightBarrierEdge(unit.edgeOnPosition + objectOffset - lightBarrierOffset, i);
						}
					}
				}

				unit.lastBarrierState = lightBarrierBit;
			}
		}
	}

	if ((interruptFlags & IF_PULSE_TIMER) != 0){
		__int64 actualTime = GetCurrentTimer();

		for (int ejectorIndex = 0; ejectorIndex < m_params.ejectorsCount; ++ejectorIndex){
			EjectorInfo& ejector = m_ejectors[ejectorIndex];

			if ((ejector.overloadCounter > 0) && (actualTime > ejector.ejectionOnTime + ejector.maxEjectorOnTime)){
				ejector.overloadCounter = 0;

				SetEjectorBit(ejectorIndex, false);
			}
		}

		for (int unitIndex = 0; unitIndex < m_params.unitsCount; ++unitIndex){
			InspectionUnitElement& unit = m_inspectionUnits[unitIndex];

			if (unit.isTriggerBitSet && (actualTime > unit.triggerOnTime + unit.triggerDuration)){
				unit.isTriggerBitSet = false;

				SetTriggerBit(unitIndex, false);
			}
		}
	}
}


// protected methods

int CTracerDriverBase::PushNewObject(I_DWORD basePosition)
{
	int inspectionIndex = m_objectsFifo.Insert(basePosition);

	if (inspectionIndex >= 0){
		ObjectInfo& objectInfo = m_objectsFifo.GetObjectAt(inspectionIndex);

		objectInfo.objectIndex = m_lastInspectedObjectIndex;
		objectInfo.decidedEjectorIndex = -1;
		objectInfo.objectState = OS_INIT;

		if (m_params.autonomeEjectorIndex >= 0){
			CTracerCommands::EjectorParams& ejector = m_ejectors[m_params.autonomeEjectorIndex];
			if (ejector.isEnabled){	// Default ejector will be used, if it is enabled
				objectInfo.decidedEjectorIndex = m_params.autonomeEjectorIndex;
			}
		}

		for (int unitIndex = 0; unitIndex < m_params.unitsCount; ++unitIndex){
			InspectionInfo& inspectionInfo = objectInfo.units[unitIndex];

			inspectionInfo.inspectionState = IS_INIT;
			inspectionInfo.ejectorIndex = -1;
			inspectionInfo.nativeTimeStamp = 0;
		}
	}
	else{
		AppendMessage(	CGeneralInfoCommands::MC_CRITICAL,
					CTracerCommands::MI_QUEUE_ERROR,
					"Objects queue is full",
					true);
		return -1;
	}

	++m_lastInspectedObjectIndex;

	return inspectionIndex;
}


void CTracerDriverBase::OnLightBarrierEdge(I_DWORD basePosition, int unitIndex)
{
	if ((m_queueEndPosition <= 0) || (m_controllerMode != CTracerCommands::TM_AUTOMATIC)){
		return;
	}

	int inspectionIndex;
	if (unitIndex == m_firstStationIndex){
		inspectionIndex = PushNewObject(basePosition);
		if (inspectionIndex < 0){
			return;
		}

		if (m_decisionEventPosition >= 0){
			InsertPositionToQueue(EI_DECISION, basePosition + m_decisionEventPosition, (void*)inspectionIndex);
		}

		if (m_params.ecLightBarrierIndex >= 0){
			int lightBarrierOffset = m_lightBarriersOffset[m_params.ecLightBarrierIndex];
			I_ASSERT(m_queueEndPosition >= lightBarrierOffset);

			InsertPositionToQueue(EI_EJECTION_CONTROL, basePosition + lightBarrierOffset, (void*)inspectionIndex);
		}

		I_ASSERT(m_queueEndPosition >= 0);
		I_ASSERT(m_queueEndPosition >= m_decisionEventPosition);
		InsertPositionToQueue(EI_REMOVE_FROM_QUEUE, basePosition + m_queueEndPosition, NULL);
	}
	else{
		inspectionIndex = m_objectsFifo.FindByPosition(basePosition, m_params.positionTolerance);

		if (inspectionIndex < 0){
			AppendMessage(CGeneralInfoCommands::MC_ERROR,
						CInspectionUnitCommands::MI_UNIDENTIFIED_OBJECT_FOUND,
						"Unidentfied object presented",
						false);
			AppendMessage(
						CGeneralInfoCommands::MC_ERROR,
						CInspectionUnitCommands::MI_UNIDENTIFIED_OBJECT_FOUND,
						": Unit ",
						false);
				char numberText[2];
				numberText[0] = '1' + char(unitIndex);
				numberText[1] = '\0';
			AppendMessage(
						CGeneralInfoCommands::MC_ERROR,
						CInspectionUnitCommands::MI_UNIDENTIFIED_OBJECT_FOUND,
						numberText,
						true);

			return;
		}
	}

	I_ASSERT(inspectionIndex >= 0);

	InspectionUnitElement& unit = m_inspectionUnits[unitIndex];

	if (unit.lightBarrierIndex >= 0){
		int triggerPosition = basePosition + m_lightBarriersOffset[unit.lightBarrierIndex] + unit.triggerOffset;

		InsertPositionToQueue(EI_TRIGGER + unitIndex, triggerPosition, (void*)inspectionIndex);
	}
}


// position events

void CTracerDriverBase::OnDecisionEvent(int inspectionIndex)
{
	ObjectInfo& objectInfo = m_objectsFifo.GetObjectAt(inspectionIndex);

	int ejectorIndex = -1;

	for (int i = 0; i < m_params.unitsCount; ++i){	// the objects for which ejection decision is made should not be taken to inspection
		InspectionUnitElement& unit = m_inspectionUnits[i];

		if (unit.lightBarrierIndex >= 0){
			InspectionInfo& inspectionInfo = objectInfo.units[i];

			int inspectionEjectorIndex = (inspectionInfo.inspectionState >= IS_RESULT)?
						inspectionInfo.ejectorIndex:
						objectInfo.decidedEjectorIndex;

			if (inspectionEjectorIndex >= 0){
				EjectorInfo& ejector = m_ejectors[inspectionEjectorIndex];

				if (ejector.isEnabled && (inspectionEjectorIndex > ejectorIndex)){
					ejectorIndex = inspectionEjectorIndex;
				}
			}
		}
	}
	I_ASSERT(ejectorIndex < m_params.ejectorsCount);

	objectInfo.decidedEjectorIndex = ejectorIndex;

	if (ejectorIndex >= 0){
		const CTracerCommands::EjectorParams& ejector = m_ejectors[ejectorIndex];

		I_DWORD basePosition = m_objectsFifo.GetPositionAt(inspectionIndex);
		I_DWORD ejectorPosition = basePosition + ejector.position;
		InsertPositionToQueue(
						m_ejectorOnEventIndex + ejectorIndex,
						ejectorPosition,
						NULL);
		InsertPositionToQueue(
						m_ejectorOffEventIndex + ejectorIndex,
						ejectorPosition + ejector.onDistance,
						NULL);
	}

	objectInfo.objectState = OS_EJECTION_DECIDED;
}


void CTracerDriverBase::OnEjectionControlEvent(int inspectionIndex)
{
	ObjectInfo& objectInfo = m_objectsFifo.GetObjectAt(inspectionIndex);

	I_ASSERT(m_params.ecLightBarrierIndex >= 0);
	bool objectPresent = GetLightBarrierBit(m_params.ecLightBarrierIndex);

	if (objectInfo.decidedEjectorIndex >= 0){
		if (objectPresent){
			AppendMessage(
						CGeneralInfoCommands::MC_ERROR,
						CTracerCommands::MI_OBJECT_UNEJECTED,
						"Object unejected",
						true);
		}
	}
	else{
		if (!objectPresent){
			AppendMessage(
						CGeneralInfoCommands::MC_ERROR,
						CTracerCommands::MI_OBJECT_EJECTED,
						"Object ejected",
						false);
		}
	}
}


void CTracerDriverBase::OnPopFifoEvent()
{
	m_objectsFifo.PopBack();
}


void CTracerDriverBase::OnTriggerEvent(int unitIndex, int inspectionIndex)
{
	I_ASSERT(unitIndex >= 0);
	I_ASSERT(unitIndex < m_params.unitsCount);

	ObjectInfo& objectInfo = m_objectsFifo.GetObjectAt(inspectionIndex);

	InspectionUnitElement& unit = m_inspectionUnits[unitIndex];
	InspectionInfo& inspectionInfo = objectInfo.units[unitIndex];

	__int64 actualTime = GetCurrentTimer();

	if (		!unit.isTriggerBitSet &&
				(m_controllerMode == CTracerCommands::TM_AUTOMATIC) &&
				(actualTime > unit.triggerOnTime + unit.triggerRelaxationTime)){
		SetTriggerBit(unitIndex, true);

		unit.triggerOnTime = actualTime;
		unit.isTriggerBitSet = true;

		inspectionInfo.nativeTimeStamp = GetCurrentNativeTimer();
		inspectionInfo.inspectionState = IS_TRIGGERED;
	}
	else{
		inspectionInfo.inspectionState = IS_UNTRIGGERED;
	}
}


void CTracerDriverBase::OnEjectorOnEvent(int ejectorIndex)
{
	I_ASSERT(ejectorIndex >= 0);
	I_ASSERT(ejectorIndex < m_params.ejectorsCount);

	EjectorInfo& ejector = m_ejectors[ejectorIndex];

	if (ejector.overloadCounter++ == 0){
		ejector.ejectionOnTime = GetCurrentTimer();

		SetEjectorBit(ejectorIndex, true);
	}
}


void CTracerDriverBase::OnEjectorOffEvent(int ejectorIndex)
{
	I_ASSERT(ejectorIndex >= 0);
	I_ASSERT(ejectorIndex < m_params.ejectorsCount);

	EjectorInfo& ejector = m_ejectors[ejectorIndex];

	if (ejector.overloadCounter > 0){
		SetEjectorBit(ejectorIndex, false);

		--ejector.overloadCounter;
	}
}


void CTracerDriverBase::OnSingleTriggerCommand(
			const CTracerCommands::SingleTrigger& command,
			CTracerCommands::SingleTrigger::Result& result)
{
	if (		(command.unitIndex < m_params.unitsCount) &&
				(m_controllerMode == CTracerCommands::TM_MANUAL)){
		InspectionUnitElement& unit = m_inspectionUnits[command.unitIndex];
		if (!unit.isTriggerBitSet){
			__int64 actualTime = GetCurrentTimer();

			unit.triggerOnTime = actualTime;
			unit.isTriggerBitSet = true;

			SetTriggerBit(command.unitIndex, true);

			result.isDone = true;
			result.nativeTimestamp = GetCurrentNativeTimer();

			return;
		}

		result.isDone = false;
	}

	if (m_controllerMode != CTracerCommands::TM_MANUAL){
		AppendMessage(CGeneralInfoCommands::MC_ERROR,
					CGeneralInfoCommands::MI_INTERNAL_ERROR,
					"Manual trigger can be done only in manual mode",
					false);
	}
	else{
		AppendMessage(CGeneralInfoCommands::MC_ERROR,
					CGeneralInfoCommands::MI_INTERNAL_ERROR,
					"Bad unit index",
					false);
	}

	AppendMessage(
				CGeneralInfoCommands::MC_ERROR,
				CInspectionUnitCommands::MI_UNIDENTIFIED_OBJECT_FOUND,
				": Unit ",
				false);
		char numberText[2];
		numberText[0] = '1' + char(command.unitIndex);
		numberText[1] = '\0';
	AppendMessage(
				CGeneralInfoCommands::MC_ERROR,
				CInspectionUnitCommands::MI_UNIDENTIFIED_OBJECT_FOUND,
				numberText,
				true);
}


} // namespace ilolv


