#include "ilolv/CTracerDriverBase.h"


#include "ilolv/CGeneralInfoMessages.h"


namespace ilolv
{


CTracerDriverBase::CTracerDriverBase()
:	m_popFifoOffset(-1)
{
	m_params.unitsCount = 0;
	m_params.ejectorsCount = 0;
	m_params.autonomeEjectorIndex = -1;
	m_params.minObjectSize = 200;
	m_params.minObjectsDistance = 300;
	m_params.positionTolerance = 10;
	m_params.ioBitDuration = 10000;
	m_params.isEcEnabled = false;
	m_params.ecLightBarrier.offset = 1000;

	for (int unitIndex = 0; unitIndex < MAX_STATIONS; ++unitIndex){
		InspectionUnitElement& station = m_inspectionUnits[unitIndex];

		station.lightBarrier.index = unitIndex;
		station.lightBarrier.offset = 0;
		station.triggerOffset = 0;
		station.triggerDuration = 10000;
		station.triggerRelaxationTime = 20000;

		station.lastTriggerTime = 0;
		station.pullTriggerDown = false;
		station.lastTriggeredIndex = 0;
		station.toInspectFifoIndex = 0;
		station.lastEdgePosition = 0;
		station.lastBarrierState = true;
	}

	for (int ejectorIndex = 0; ejectorIndex < MAX_EJECTORS; ++ejectorIndex){
		EjectorInfo& ejector = m_ejectors[ejectorIndex];

		ejector.isEnabled = false;
		ejector.position = 0;
		ejector.onDistance = 200;
		ejector.maxEjectorOnTime = 10000;

		ejector.ejectorOnCount = 0;
		ejector.lastEjectorTime = 0;
	}

	m_fifoLastIndex = 0;
	m_fifoNextIndex = 0;
	m_nextTriggerOffsetIndex = 0;
	m_ejectionDecisionIndex = 0;
	m_pointControlIndex = 0;
	m_lastInspectedObjectIndex = 0;
	m_lastFoundObjectIndex = 0;

	m_controllerMode = CM_MANUAL;

	m_firstEjectorOnEvent = PE_FIRST_TRIGGER;
	m_firstEjectorOffEvent = PE_FIRST_TRIGGER;
	m_firstStationIndex = 0;
}


void CTracerDriverBase::ResetQueue()
{
	m_fifoLastIndex = 0;
	m_fifoNextIndex = 0;
	m_nextTriggerOffsetIndex = 0;
	m_ejectionDecisionIndex = 0;
	m_pointControlIndex = 0;

	I_DWORD linePosition = GetLinePosition();

	int maxPopFifoDistance = 0;
	for (int unitIndex = 0; unitIndex < m_params.unitsCount; ++unitIndex){
		InspectionUnitElement& station = m_inspectionUnits[unitIndex];

		station.lastTriggerTime = 0;
		station.pullTriggerDown = 0;
		station.lastTriggeredIndex = 0;
		station.toInspectFifoIndex = 0;
		station.lastEdgePosition =
						linePosition +
						station.lightBarrier.offset -
						m_params.minObjectsDistance;
		station.lastBarrierState = true;

		int triggerOffset = station.lightBarrier.offset + station.triggerOffset + 1;
		if (triggerOffset > maxPopFifoDistance){
			maxPopFifoDistance = triggerOffset;
		}

		SetTriggerBit(unitIndex, false);
	}

	int minEjectorOnDistance = -1;
	for (int ejectorIndex = 0; ejectorIndex < m_params.ejectorsCount; ++ejectorIndex){
		EjectorInfo& ejector = m_ejectors[ejectorIndex];

		ejector.ejectorOnCount = 0;
		ejector.lastEjectorTime = 0;

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

	m_firstEjectorOnEvent = PE_FIRST_TRIGGER + m_params.unitsCount;
	m_firstEjectorOffEvent = m_firstEjectorOnEvent + m_params.ejectorsCount;

	m_ejectionDecisionOffset = minEjectorOnDistance;
	m_popFifoOffset = maxPopFifoDistance;

	if (!SetCounterQueuesCount(m_firstEjectorOffEvent + m_params.ejectorsCount)){
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


int CTracerDriverBase::PopInspectionId(int unitIndex)
{
	if (unitIndex < m_params.unitsCount){
		InspectionUnitElement& station = m_inspectionUnits[unitIndex];
		while (station.toInspectFifoIndex != m_fifoNextIndex){
			int fifoIndex = station.toInspectFifoIndex;

			TriggerType triggerType = m_fifo[fifoIndex].usedTriggers[unitIndex];
			if ((triggerType == TT_UNKNOWN) && (m_controllerMode == CM_AUTOMATIC)){
				return -1;
			}

			station.toInspectFifoIndex = CalcNextFifoIndex(fifoIndex);

			if ((triggerType == TT_TRIGGER) || (m_controllerMode != CM_AUTOMATIC)){
				return fifoIndex;
			}
		}
	}

	return -1;
}


I_DWORD CTracerDriverBase::GetObjectPosition(int inspectionId) const
{
	I_ASSERT(inspectionId >= 0);
	I_ASSERT(inspectionId < FIFO_COUNT);

	return m_fifo[inspectionId].basePosition;
}


I_DWORD CTracerDriverBase::GetObjectIndex(int inspectionId) const
{
	I_ASSERT(inspectionId >= 0);
	I_ASSERT(inspectionId < FIFO_COUNT);

	return m_fifo[inspectionId].objectIndex;
}


bool CTracerDriverBase::SetInspectionResult(int /*unitIndex*/, int inspectionId, int ejectorIndex)
{
	I_ASSERT(inspectionId >= 0);
	I_ASSERT(inspectionId < FIFO_COUNT);
	I_ASSERT(ejectorIndex < m_params.ejectorsCount);

	if ((inspectionId >= 0) && (inspectionId < FIFO_COUNT)){
		FifoElement& element = m_fifo[inspectionId];
		if (element.objectState >= OS_DECIDED){		// ejection decision was made, it is too late
			return false;
		}

		if (ejectorIndex >= 0){
			EjectorInfo& ejector = m_ejectors[ejectorIndex];

			if ((ejectorIndex > element.ejectorIndex) && ejector.isEnabled){
				element.ejectorIndex = ejectorIndex;
			}
		}
		else{
			element.okCount++;
		}
	}

	return true;
}


void CTracerDriverBase::OnPositionEvent(int eventIndex)
{
	if (eventIndex < PE_FIRST_TRIGGER){
		switch (eventIndex){
		case PE_POP_FIFO:
			OnPositionPopFifo();
			break;

		case PE_EJECTION_DECISION:
			OnPositionEjectionDecision();
			break;

		case PE_EJECTION_CONTROL:
			OnPointControl();
			break;
		}
	}
	else{
		if (eventIndex < m_firstEjectorOnEvent){
			// trigger event, called at position where object should be triggered
			int unitIndex = eventIndex - PE_FIRST_TRIGGER;
			I_ASSERT(unitIndex < m_params.unitsCount);

			OnPositionTrigger(unitIndex);
		}
		else if (eventIndex < m_firstEjectorOffEvent){
			// "ejector on" event, called at position where ejector should be turned on
			int ejectorIndex = eventIndex - m_firstEjectorOnEvent;
			I_ASSERT(ejectorIndex < m_params.ejectorsCount);

			OnPositionEjectorOn(ejectorIndex);
		}
		else if (eventIndex < m_firstEjectorOffEvent + m_params.ejectorsCount){
			// "ejector off" event, called at position where ejector should be turned off
			int ejectorIndex = eventIndex - m_firstEjectorOffEvent;
			I_ASSERT(ejectorIndex < m_params.ejectorsCount);

			OnPositionEjectorOff(ejectorIndex);
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

			if (m_params.unitsCount > MAX_STATIONS){
				m_params.unitsCount = MAX_STATIONS;
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
			result.lastDetectedObjectIndex = m_lastFoundObjectIndex;

			responseSize = sizeof(CTracerMessages::GetLineInfo::Result);
		}
		break;

	case CTracerMessages::PopId::Id:
		if (		(instructionBufferSize >= sizeof(CTracerMessages::PopId)) &&
					(responseBufferSize >= sizeof(CTracerMessages::PopId::Result))){
			const CTracerMessages::PopId& instruction = *(const CTracerMessages::PopId*)instructionBuffer;
			CTracerMessages::PopId::Result& result = *(CTracerMessages::PopId::Result*)responseBuffer;

			int inspectionId = PopInspectionId(instruction.unitIndex);

			result.inspectionId = inspectionId;
			if (inspectionId >= 0){
				result.nativeTimestamp = m_fifo[inspectionId].nativeTimeStamps[instruction.unitIndex];
				result.objectIndex = GetObjectIndex(inspectionId);
				result.objectPosition = GetObjectPosition(inspectionId);
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

				result.wasSet = SetInspectionResult(
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
					if (I_SDWORD(linePosition - unit.lastEdgePosition - m_params.minObjectsDistance) > 0){
						OnLightBarrierEdge(linePosition - unit.lightBarrier.offset, i);
					}
				}

				unit.lastEdgePosition = linePosition;
			}
			else if (!lightBarrierBit && unit.lastBarrierState){
				if (unit.edgePosition != 0){
					I_DWORD objectSize = linePosition - unit.lastEdgePosition;
					if (int(objectSize) < m_params.minObjectSize){
						I_DWORD objectOffset = objectSize * unit.edgePosition / CInspectionUnitMessages::UnitParams::FALLING_EDGE;

						OnLightBarrierEdge(unit.lastEdgePosition + objectOffset - unit.lightBarrier.offset, i);
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

		if ((ejector.ejectorOnCount > 0) && (actualTime > ejector.lastEjectorTime + ejector.maxEjectorOnTime)){
			ejector.ejectorOnCount = 0;

			SetEjectorBit(ejectorIndex, false);
		}
	}

	for (int unitIndex = 0; unitIndex < m_params.unitsCount; ++unitIndex){
		InspectionUnitElement& station = m_inspectionUnits[unitIndex];

		if (station.pullTriggerDown && (actualTime > station.lastTriggerTime + station.triggerDuration)){
			station.pullTriggerDown = false;

			SetTriggerBit(unitIndex, false);
		}
	}

	for (int ioBitIndex = 0; ioBitIndex <= IB_LAST; ++ioBitIndex){
		__int64& lastIoBitTime = m_lastIoBitTimes[ioBitIndex];

		if ((lastIoBitTime != 0) && (actualTime > lastIoBitTime + m_params.ioBitDuration)){
			lastIoBitTime = 0;

			SetIoBit(ioBitIndex, false);
		}
	}
}


// protected methods

void CTracerDriverBase::OnLightBarrierEdge(I_DWORD basePosition, int unitIndex)
{
	InspectionUnitElement& unit = m_inspectionUnits[unitIndex];

	if (unitIndex == m_firstStationIndex){
		if (AddObjectToFifo(basePosition)){
			if (m_controllerMode == CM_AUTOMATIC){
				++m_lastInspectedObjectIndex;

				if (m_ejectionDecisionOffset >= 0){
					InsertPositionToQueue(PE_EJECTION_DECISION, basePosition + m_ejectionDecisionOffset);
				}

				if (m_params.isEcEnabled){
					InsertPositionToQueue(PE_EJECTION_CONTROL, basePosition + m_params.ecLightBarrier.offset);
				}
			}

			I_ASSERT(m_popFifoOffset >= 0);
			InsertPositionToQueue(PE_POP_FIFO, basePosition + m_popFifoOffset);
		}
	}

	if ((m_controllerMode == CM_AUTOMATIC) && (unit.triggerOffset >= 0)){
		I_DWORD triggerPosition = basePosition + unit.lightBarrier.offset + unit.triggerOffset;
		InsertPositionToQueue(PE_FIRST_TRIGGER + unitIndex, triggerPosition);
	}
}


bool CTracerDriverBase::AddObjectToFifo(I_DWORD basePosition)
{
	if (m_popFifoOffset <= 0){
		return false;	// FIFO is not initialized
	}

	bool retVal = true;

	++m_lastFoundObjectIndex;

	FifoElement& element = m_fifo[m_fifoNextIndex];
	element.okCount = 0;
	element.basePosition = basePosition;
	element.objectIndex = m_lastInspectedObjectIndex;
	element.ejectorIndex = -1;

	if (m_params.autonomeEjectorIndex >= 0){
		CTracerMessages::EjectorParams& ejector = m_ejectors[m_params.autonomeEjectorIndex];
		if (ejector.isEnabled){	// Default ejector will be used, if it is enabled
			element.ejectorIndex = m_params.autonomeEjectorIndex;
		}
	}

	if (m_controllerMode == CM_AUTOMATIC){
		element.objectState = OS_INIT;
	}
	else{
		element.objectState = OS_DIAGNOSTIC;
	}

	for (int unitIndex = 0; unitIndex < m_params.unitsCount; ++unitIndex){
		element.usedTriggers[unitIndex] = TT_UNKNOWN;
		element.nativeTimeStamps[unitIndex] = 0;
	}

	int newFifoBegin = CalcNextFifoIndex(m_fifoNextIndex);
	if (newFifoBegin == m_fifoLastIndex){
		AppendMessage(	CGeneralInfoMessages::MC_CRITICAL,
					CTracerMessages::MI_QUEUE_ERROR,
					"Objects queue is full",
					true);

		PopFifoLast();

		retVal = false;
	}


	I_ASSERT(newFifoBegin != m_fifoLastIndex);
	m_fifoNextIndex = newFifoBegin;

	return retVal;
}


int CTracerDriverBase::FindInFifo(I_DWORD basePosition)
{
	int fifoIndex = m_fifoLastIndex;
	while (fifoIndex != m_fifoNextIndex){
		I_SDWORD posDiff = I_SDWORD(basePosition - m_fifo[fifoIndex].basePosition);
		if ((posDiff > -I_SDWORD(m_params.positionTolerance)) && (posDiff < I_SDWORD(m_params.positionTolerance))){
			return fifoIndex;
		}

		fifoIndex = CalcNextFifoIndex(fifoIndex);
	}

	return -1;
}


const CTracerDriverBase::FifoElement& CTracerDriverBase::PopFifoLast()
{
	int newFifoLast = CalcNextFifoIndex(m_fifoLastIndex);

	for (int unitIndex = 0; unitIndex < m_params.unitsCount; ++unitIndex){
		InspectionUnitElement& station = m_inspectionUnits[unitIndex];
		if (station.toInspectFifoIndex == m_fifoLastIndex){
			station.toInspectFifoIndex = newFifoLast;
		}
	}

	if (m_nextTriggerOffsetIndex == m_fifoLastIndex){
		m_nextTriggerOffsetIndex = newFifoLast;
	}
	if (m_ejectionDecisionIndex == m_fifoLastIndex){
		m_ejectionDecisionIndex = newFifoLast;
	}
	if (m_pointControlIndex == m_fifoLastIndex){
		m_pointControlIndex = newFifoLast;
	}

	const FifoElement& retVal = m_fifo[m_fifoLastIndex];

	m_fifoLastIndex = newFifoLast;

	return retVal;
}


// position events

void CTracerDriverBase::OnPositionTrigger(int unitIndex)
{
	I_ASSERT(unitIndex >= 0);
	I_ASSERT(unitIndex < m_params.unitsCount);

	InspectionUnitElement& station = m_inspectionUnits[unitIndex];

	int fifoIndex = -1;

	if (unitIndex == m_firstStationIndex){
		fifoIndex = m_nextTriggerOffsetIndex;
		m_nextTriggerOffsetIndex = CalcNextFifoIndex(fifoIndex);
	}
	else{
		I_DWORD linePosition = GetLinePosition();

		I_DWORD basePosition = linePosition - station.lightBarrier.offset - station.triggerOffset;

		fifoIndex = FindInFifo(basePosition);
	}

	if (fifoIndex >= 0){
		FifoElement& element = m_fifo[fifoIndex];

		__int64 actualTime = GetCurrentTimer();
		bool useTrigger =
						!station.pullTriggerDown &&
						(m_controllerMode == CM_AUTOMATIC) &&
						(actualTime > station.lastTriggerTime + station.triggerRelaxationTime);

		element.usedTriggers[unitIndex] = useTrigger? TT_TRIGGER: TT_LOST;

		if (useTrigger){
			NativeTimer internalTimer = GetCurrentNativeTimer();
			element.nativeTimeStamps[unitIndex] = internalTimer;

			station.lastTriggerTime = actualTime;
			station.pullTriggerDown = true;

			SetTriggerBit(unitIndex, true);
		}
	}
	else{
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
	}
}


void CTracerDriverBase::OnPositionEjectionDecision()
{
	FifoElement& element = m_fifo[m_ejectionDecisionIndex];

	int newDecisionIndex = CalcNextFifoIndex(m_ejectionDecisionIndex);

	for (int i = 0; i < m_params.unitsCount; ++i){	// the objects for which ejection decision is made should not be taken to inspection
		InspectionUnitElement& station = m_inspectionUnits[i];

		if (station.toInspectFifoIndex == m_ejectionDecisionIndex){
			station.toInspectFifoIndex = newDecisionIndex;
		}
	}

	if (element.okCount < m_params.unitsCount){
		int ejectorIndex = element.ejectorIndex;
		if (ejectorIndex >= 0){
			if (element.objectState < OS_EJECTED){
				element.objectState = OS_EJECTED;
			}

			I_ASSERT(ejectorIndex < m_params.ejectorsCount);

			const CTracerMessages::EjectorParams& ejector = m_ejectors[ejectorIndex];

			I_DWORD ejectorPosition = element.basePosition + ejector.position;
			InsertPositionToQueue(
							m_firstEjectorOnEvent + ejectorIndex,
							ejectorPosition);
			InsertPositionToQueue(
							m_firstEjectorOffEvent + ejectorIndex,
							ejectorPosition + ejector.onDistance);
		}
	}

	if (element.objectState < OS_DECIDED){
		element.objectState = OS_DECIDED;
	}

	m_ejectionDecisionIndex = newDecisionIndex;
}


void CTracerDriverBase::OnPointControl()
{
	FifoElement& element = m_fifo[m_pointControlIndex];

	I_ASSERT(m_params.ecLightBarrier.index >= 0);
	bool objectPresent = GetLightBarrierBit(m_params.ecLightBarrier.index);
	bool shouldBeEjected = ((element.okCount < m_params.unitsCount) && (element.ejectorIndex >= 0));
	if (shouldBeEjected){
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

	m_pointControlIndex = CalcNextFifoIndex(m_pointControlIndex);
}


void CTracerDriverBase::OnPositionEjectorOn(int ejectorIndex)
{
	I_ASSERT(ejectorIndex >= 0);
	I_ASSERT(ejectorIndex < m_params.ejectorsCount);

	EjectorInfo& ejector = m_ejectors[ejectorIndex];

	if (ejector.ejectorOnCount++ == 0){
		__int64 actualTime = GetCurrentTimer();

		ejector.lastEjectorTime = actualTime;

		SetEjectorBit(ejectorIndex, true);
	}
}


void CTracerDriverBase::OnPositionEjectorOff(int ejectorIndex)
{
	I_ASSERT(ejectorIndex >= 0);
	I_ASSERT(ejectorIndex < m_params.ejectorsCount);

	EjectorInfo& ejector = m_ejectors[ejectorIndex];

	if (ejector.ejectorOnCount > 0){
		--ejector.ejectorOnCount;

		SetEjectorBit(ejectorIndex, false);
	}
}


void CTracerDriverBase::OnPositionPopFifo()
{
	const FifoElement& element = PopFifoLast();

	bool shouldBeEjected = ((element.okCount < m_params.unitsCount) && (element.ejectorIndex >= 0));

	int signalIndex = shouldBeEjected? IB_NOK_SIGNAL: IB_OK_SIGNAL;

	__int64 actualTime = GetCurrentTimer();
	m_lastIoBitTimes[signalIndex] = actualTime;

	SetIoBit(signalIndex, true);
}


void CTracerDriverBase::OnSingleTriggerInstruction(
			const CTracerMessages::SingleTrigger& instruction,
			CTracerMessages::SingleTrigger::Result& result)
{
	if (		(instruction.unitIndex < m_params.unitsCount) &&
				(m_controllerMode != CM_AUTOMATIC)){
		InspectionUnitElement& station = m_inspectionUnits[instruction.unitIndex];
		if (!station.pullTriggerDown){
			__int64 actualTime = GetCurrentTimer();

			station.lastTriggerTime = actualTime;
			station.pullTriggerDown = true;

			SetTriggerBit(instruction.unitIndex, true);

			result.nativeTimestamp = GetCurrentNativeTimer();

			return;
		}
	}

	if (m_controllerMode == CM_AUTOMATIC){
		AppendMessage(CGeneralInfoMessages::MC_ERROR,
					CGeneralInfoMessages::MI_INTERNAL_ERROR,
					"Manual trigger in automatic mode",
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


