#include "ilolv/CTracerDriverBase.h"


#include "ilolv/CGeneralInfoMessages.h"


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
	m_params.isEcEnabled = false;
	m_params.ecLightBarrier.offset = 1000;

	for (int unitIndex = 0; unitIndex < MAX_UNITS_COUNT; ++unitIndex){
		InspectionUnitElement& unit = m_inspectionUnits[unitIndex];

		unit.lightBarrier.index = unitIndex;
		unit.lightBarrier.offset = 0;
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

	m_lastInspectedObjectIndex = 0;

	m_controllerMode = CTracerMessages::TM_DISABLED;

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
		unit.edgeOnPosition =
						linePosition +
						unit.lightBarrier.offset -
						m_params.minObjectsDistance;
		unit.lastBarrierState = true;

		int triggerOffset = unit.lightBarrier.offset + unit.triggerOffset + 1;
		if (triggerOffset > maxPopFifoDistance){
			maxPopFifoDistance = triggerOffset;
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

	if (m_params.isEcEnabled){
		if (m_params.ecLightBarrier.offset > maxPopFifoDistance){
			maxPopFifoDistance = m_params.ecLightBarrier.offset;
		}
	}

	m_ejectorOnEventIndex = EI_TRIGGER + m_params.unitsCount;
	m_ejectorOffEventIndex = m_ejectorOnEventIndex + m_params.ejectorsCount;

	m_decisionEventPosition = minEjectorOnDistance;
	m_queueEndPosition = maxPopFifoDistance;

	if (!SetCounterQueuesCount(m_ejectorOffEventIndex + m_params.ejectorsCount)){
		AppendMessage(CGeneralInfoMessages::MC_CRITICAL,
					CGeneralInfoMessages::MI_INTERNAL_ERROR,
					"More queues needed than supported",
					true);
	}
}


const CInspectionUnitMessages::UnitParams& CTracerDriverBase::GetUnitParams(int unitIndex) const
{
	I_ASSERT(unitIndex >= 0);
	I_ASSERT(unitIndex < m_params.unitsCount);

	return m_inspectionUnits[unitIndex];
}


const CTracerMessages::EjectorParams& CTracerDriverBase::GetEjectorParams(int ejectorIndex) const
{
	I_ASSERT(ejectorIndex >= 0);
	I_ASSERT(ejectorIndex < m_params.ejectorsCount);
	I_ASSERT(m_params.ejectorsCount <= MAX_EJECTORS);

	return m_ejectors[ejectorIndex];
}


int CTracerDriverBase::OnPopInspectionCommand(int unitIndex)
{
	if ((m_controllerMode == CTracerMessages::TM_AUTOMATIC) && (unitIndex < m_params.unitsCount)){
		InspectionUnitElement& unit = m_inspectionUnits[unitIndex];

		if (unit.isEnabled){
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

bool CTracerDriverBase::OnInstruction(
			I_DWORD instructionCode,
			const void* instructionBuffer,
			int instructionBufferSize,
			void* responseBuffer,
			int responseBufferSize,
			I_DWORD& responseSize)
{
	responseSize = 0;

	switch (instructionCode){
	case CTracerMessages::SetParams::Id:
		if (instructionBufferSize >= sizeof(CTracerMessages::SetParams)){
			m_params = *(const CTracerMessages::SetParams*)instructionBuffer;

			if (m_params.unitsCount > MAX_UNITS_COUNT){
				m_params.unitsCount = MAX_UNITS_COUNT;
			}

			if (m_params.ejectorsCount > MAX_EJECTORS){
				m_params.ejectorsCount = MAX_EJECTORS;
			}

			if (m_params.autonomeEjectorIndex >= m_params.ejectorsCount){
				m_params.autonomeEjectorIndex = m_params.ejectorsCount - 1;
			}

			if (m_params.ecLightBarrier.index >= m_params.lightBarriersCount){
				m_params.ecLightBarrier.index = -1;
			}
		}
		break;

	case CTracerMessages::SetUnitParams::Id:
		if (instructionBufferSize >= sizeof(CTracerMessages::SetUnitParams)){
			const CTracerMessages::SetUnitParams& instruction = *(const CTracerMessages::SetUnitParams*)instructionBuffer;

			if ((instruction.unitIndex >= 0) && (instruction.unitIndex < m_params.unitsCount)){
				CInspectionUnitMessages::UnitParams& params = m_inspectionUnits[instruction.unitIndex];

				params = instruction.unit;

				if (params.lightBarrier.index >= m_params.lightBarriersCount){
					params.lightBarrier.index = 1;
				}
			}
		}
		break;

	case CTracerMessages::SetEjectorParams::Id:
		if (instructionBufferSize >= sizeof(CTracerMessages::SetEjectorParams)){
			const CTracerMessages::SetEjectorParams& instruction = *(const CTracerMessages::SetEjectorParams*)instructionBuffer;

			int ejectorIndex = int(instruction.ejectorIndex);
			if ((ejectorIndex >= 0) && (ejectorIndex < m_params.ejectorsCount)){
				CTracerMessages::EjectorParams& params = m_ejectors[ejectorIndex];

				params = instruction.ejector;
			}
		}
		break;

	case CTracerMessages::SetMode::Id:
		if (instructionBufferSize >= sizeof(CTracerMessages::SetMode)){
			const CTracerMessages::SetMode& instruction = *(const CTracerMessages::SetMode*)instructionBuffer;

			m_controllerMode = instruction.mode;

			ResetQueue();
		}
		break;

	case CTracerMessages::SingleTrigger::Id:
		if (		(instructionBufferSize >= sizeof(CTracerMessages::SingleTrigger)) &&
					(responseBufferSize >= sizeof(CTracerMessages::SingleTrigger::Result))){
			OnSingleTriggerInstruction(
						*(const CTracerMessages::SingleTrigger*)instructionBuffer,
						*(CTracerMessages::SingleTrigger::Result*)responseBuffer);
			responseSize = sizeof(CTracerMessages::SingleTrigger::Result);
		}
		break;

	case CTracerMessages::GetLineInfo::Id:
		if (responseBufferSize >= sizeof(CTracerMessages::GetLineInfo::Result)){
			CTracerMessages::GetLineInfo::Result& result = *(CTracerMessages::GetLineInfo::Result*)responseBuffer;

			result.linePos = GetLinePosition();
			result.lastDetectedObjectIndex = m_lastInspectedObjectIndex;

			responseSize = sizeof(CTracerMessages::GetLineInfo::Result);
		}
		break;

	case CTracerMessages::PopId::Id:
		if (		(instructionBufferSize >= sizeof(CTracerMessages::PopId)) &&
					(responseBufferSize >= sizeof(CTracerMessages::PopId::Result))){
			const CTracerMessages::PopId& instruction = *(const CTracerMessages::PopId*)instructionBuffer;
			CTracerMessages::PopId::Result& result = *(CTracerMessages::PopId::Result*)responseBuffer;

			int inspectionId = OnPopInspectionCommand(instruction.unitIndex);

			result.inspectionId = inspectionId;
			if (inspectionId >= 0){
				const ObjectInfo& objectInfo = m_objectsFifo.GetObjectAt(inspectionId);

				result.nativeTimestamp = objectInfo.units[instruction.unitIndex].nativeTimeStamp;
				result.objectIndex = objectInfo.objectIndex;
				result.objectPosition = m_objectsFifo.GetPositionAt(inspectionId);
			}

			responseSize = sizeof(CTracerMessages::PopId::Result);
		}
		break;

	case CTracerMessages::SetResult::Id:
		if (		(instructionBufferSize >= sizeof(CTracerMessages::SetResult)) &&
					(responseBufferSize >= sizeof(CTracerMessages::SetResult::Result))){
			const CTracerMessages::SetResult& instruction = *(const CTracerMessages::SetResult*)instructionBuffer;
			CTracerMessages::SetResult::Result& result = *(CTracerMessages::SetResult::Result*)responseBuffer;

			if ((instruction.unitIndex >= 0) && (instruction.unitIndex < m_params.unitsCount)){
				int ejectorIndex;
				if (instruction.unit.result != 0){
					ejectorIndex = -1;
				}
				else{
					ejectorIndex = instruction.unit.ejectorIndex;
				}

				result.wasSet = OnSetResultCommand(
								instruction.unitIndex,
								instruction.unit.inspectionId,
								ejectorIndex);

				responseSize = sizeof(CTracerMessages::SetResult::Result);
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
	if ((interruptFlags & IF_ENCODER_INTERRUPT) == 0){
		return;
	}

	I_DWORD linePosition = GetLinePosition();

	for (int i = 0; i < m_params.unitsCount; ++i){
		InspectionUnitElement& unit = m_inspectionUnits[i];
		int lightBarrierIndex = unit.lightBarrier.index;
		if (lightBarrierIndex >= 0){
			bool lightBarrierBit = GetLightBarrierBit(lightBarrierIndex);
			if (lightBarrierBit && !unit.lastBarrierState){
				if (unit.edgePosition == 0){
					if (I_SDWORD(linePosition - unit.edgeOnPosition - m_params.minObjectsDistance) > 0){
						OnLightBarrierEdge(linePosition - unit.lightBarrier.offset, i);
					}
				}

				unit.edgeOnPosition = linePosition;
			}
			else if (!lightBarrierBit && unit.lastBarrierState){
				if (unit.edgePosition != 0){
					I_DWORD objectSize = linePosition - unit.edgeOnPosition;
					if (int(objectSize) < m_params.minObjectSize){
						I_DWORD objectOffset = objectSize * unit.edgePosition / CInspectionUnitMessages::UnitParams::FALLING_EDGE;

						OnLightBarrierEdge(unit.edgeOnPosition + objectOffset - unit.lightBarrier.offset, i);
					}
				}
			}

			unit.lastBarrierState = lightBarrierBit;
		}
	}
}


void CTracerDriverBase::OnPeriodicPulse()
{
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
			CTracerMessages::EjectorParams& ejector = m_ejectors[m_params.autonomeEjectorIndex];
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
		AppendMessage(	CGeneralInfoMessages::MC_CRITICAL,
					CTracerMessages::MI_QUEUE_ERROR,
					"Objects queue is full",
					true);
		return -1;
	}

	++m_lastInspectedObjectIndex;

	return inspectionIndex;
}


void CTracerDriverBase::OnLightBarrierEdge(I_DWORD basePosition, int unitIndex)
{
	if ((m_queueEndPosition <= 0) || (m_controllerMode != CTracerMessages::TM_AUTOMATIC)){
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

		if (m_params.isEcEnabled){
			InsertPositionToQueue(EI_EJECTION_CONTROL, basePosition + m_params.ecLightBarrier.offset, (void*)inspectionIndex);
		}

		I_ASSERT(m_queueEndPosition >= 0);
		I_ASSERT(m_queueEndPosition >= m_decisionEventPosition);
		I_ASSERT(m_queueEndPosition >= m_params.ecLightBarrier.offset);
		InsertPositionToQueue(EI_REMOVE_FROM_QUEUE, basePosition + m_queueEndPosition, NULL);
	}
	else{
		inspectionIndex = m_objectsFifo.FindByPosition(basePosition, m_params.positionTolerance);

		if (inspectionIndex < 0){
			AppendMessage(CGeneralInfoMessages::MC_ERROR,
						CInspectionUnitMessages::MI_UNIDENTIFIED_OBJECT_FOUND,
						"Unidentfied object presented",
						false);
			AppendMessage(
						CGeneralInfoMessages::MC_ERROR,
						CInspectionUnitMessages::MI_UNIDENTIFIED_OBJECT_FOUND,
						": Unit ",
						false);
				char numberText[2];
				numberText[0] = '1' + char(unitIndex);
				numberText[1] = '\0';
			AppendMessage(
						CGeneralInfoMessages::MC_ERROR,
						CInspectionUnitMessages::MI_UNIDENTIFIED_OBJECT_FOUND,
						numberText,
						true);

			return;
		}
	}

	I_ASSERT(inspectionIndex >= 0);

	InspectionUnitElement& unit = m_inspectionUnits[unitIndex];

	if (unit.isEnabled){
		I_DWORD triggerPosition = basePosition + unit.lightBarrier.offset + unit.triggerOffset;
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

		if (unit.isEnabled){
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
		const CTracerMessages::EjectorParams& ejector = m_ejectors[ejectorIndex];

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

	I_ASSERT(m_params.ecLightBarrier.index >= 0);
	bool objectPresent = GetLightBarrierBit(m_params.ecLightBarrier.index);

	if (objectInfo.decidedEjectorIndex >= 0){
		if (objectPresent){
			AppendMessage(
						CGeneralInfoMessages::MC_ERROR,
						CTracerMessages::MI_OBJECT_UNEJECTED,
						"Object unejected",
						true);
		}
	}
	else{
		if (!objectPresent){
			AppendMessage(
						CGeneralInfoMessages::MC_ERROR,
						CTracerMessages::MI_OBJECT_EJECTED,
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
				(m_controllerMode == CTracerMessages::TM_AUTOMATIC) &&
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


void CTracerDriverBase::OnSingleTriggerInstruction(
			const CTracerMessages::SingleTrigger& instruction,
			CTracerMessages::SingleTrigger::Result& result)
{
	if (		(instruction.unitIndex < m_params.unitsCount) &&
				(m_controllerMode == CTracerMessages::TM_MANUAL)){
		InspectionUnitElement& unit = m_inspectionUnits[instruction.unitIndex];
		if (!unit.isTriggerBitSet){
			__int64 actualTime = GetCurrentTimer();

			unit.triggerOnTime = actualTime;
			unit.isTriggerBitSet = true;

			SetTriggerBit(instruction.unitIndex, true);

			result.isDone = true;
			result.nativeTimestamp = GetCurrentNativeTimer();

			return;
		}

		result.isDone = false;
	}

	if (m_controllerMode != CTracerMessages::TM_MANUAL){
		AppendMessage(CGeneralInfoMessages::MC_ERROR,
					CGeneralInfoMessages::MI_INTERNAL_ERROR,
					"Manual trigger can be done only in manual mode",
					false);
	}
	else{
		AppendMessage(CGeneralInfoMessages::MC_ERROR,
					CGeneralInfoMessages::MI_INTERNAL_ERROR,
					"Bad unit index",
					false);
	}

	AppendMessage(
				CGeneralInfoMessages::MC_ERROR,
				CInspectionUnitMessages::MI_UNIDENTIFIED_OBJECT_FOUND,
				": Unit ",
				false);
		char numberText[2];
		numberText[0] = '1' + char(instruction.unitIndex);
		numberText[1] = '\0';
	AppendMessage(
				CGeneralInfoMessages::MC_ERROR,
				CInspectionUnitMessages::MI_UNIDENTIFIED_OBJECT_FOUND,
				numberText,
				true);
}


} // namespace ilolv


