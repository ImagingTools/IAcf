#include "ilolv/CMultiTracerDriverBase.h"


#include "ilolv/CGeneralInfoMessages.h"


namespace ilolv
{


CMultiTracerDriverBase::CMultiTracerDriverBase()
:	m_interruptsMask(0),
	m_outputBits(0),
	m_lastOutputBits(0),
	m_microsecsTimer(0),
	m_nativeTimer(0)
{
	m_params.linesCount = 0;

	for (int lineIndex = 0; lineIndex < MAX_LINES; ++lineIndex){
		m_lines[lineIndex].Init(this);
		m_lines[lineIndex].SetLineIndex(lineIndex);
	}
}


// reimplemented (ilolv::IDriver)

bool CMultiTracerDriverBase::OnInstruction(
			I_DWORD instructionCode,
			const void* instructionBuffer,
			int instructionBufferSize,
			void* responseBuffer,
			int responseBufferSize,
			I_DWORD& responseSize)
{
	I_ASSERT((instructionBuffer != NULL) || (instructionBufferSize == 0));

	responseSize = 0;

	switch (instructionCode){
	case CMultiTracerMessages::SetParams::Id:
		if (instructionBufferSize >= sizeof(CMultiTracerMessages::SetParams)){
			m_params = *(const CMultiTracerMessages::SetParams*)instructionBuffer;
			if (m_params.linesCount > MAX_LINES){
				m_params.linesCount = MAX_LINES;
			}

			CalcInterruptsMask();
		}
		break;

	case CMultiTracerMessages::SetLineParams::Id:
		if (instructionBufferSize >= sizeof(CMultiTracerMessages::SetLineParams)){
			const CMultiTracerMessages::SetLineParams& instruction = *(const CMultiTracerMessages::SetLineParams*)instructionBuffer;

			int lineIndex = int(instruction.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				line.m_lightBarrierBitsIndex = instruction.lightBarriersBitIndex;
				line.m_ejectionControlBitIndex = instruction.ejectionControlBitIndex;
				line.m_counterReadyBitIndex = instruction.readyBitIndex;
				line.m_triggersBitIndex = instruction.triggersBitIndex;
				line.m_ejectorsBitIndex = instruction.ejectorsBitIndex;
				line.m_iosBitIndex = instruction.iosBitIndex;

				line.OnInstruction(
							CTracerMessages::SetParams::Id,
							&instruction.line,
							sizeof(CMultiTracerMessages::SetLineParams),
							responseBuffer,
							responseBufferSize,
							responseSize);

				CalcInterruptsMask();
			}
		}
		break;

	case CMultiTracerMessages::SetUnitParams::Id:
		if (instructionBufferSize >= sizeof(CMultiTracerMessages::SetUnitParams)){
			const CMultiTracerMessages::SetUnitParams& instruction = *(const CMultiTracerMessages::SetUnitParams*)instructionBuffer;

			int lineIndex = int(instruction.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				line.OnInstruction(
							CTracerMessages::SetUnitParams::Id,
							&instruction.line,
							sizeof(CMultiTracerMessages::SetUnitParams),
							responseBuffer,
							responseBufferSize,
							responseSize);
			}
		}
		break;

	case CMultiTracerMessages::SetEjectorParams::Id:
		if (instructionBufferSize >= sizeof(CMultiTracerMessages::SetEjectorParams)){
			const CMultiTracerMessages::SetEjectorParams& instruction = *(const CMultiTracerMessages::SetEjectorParams*)instructionBuffer;

			int lineIndex = int(instruction.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				line.OnInstruction(
							CTracerMessages::SetEjectorParams::Id,
							&instruction.line,
							sizeof(CMultiTracerMessages::SetEjectorParams),
							responseBuffer,
							responseBufferSize,
							responseSize);
			}
		}
		break;

	case CMultiTracerMessages::SetMode::Id:
		for (int lineIndex = 0; lineIndex < MAX_LINES; ++lineIndex){
			SingleLine& line = m_lines[lineIndex];

			WriteCounter(lineIndex, 0);
			line.m_lastCounterReadValue = ReadCounter(lineIndex);
			line.m_isCounterUnknown = true;
			line.m_isCounterReadyUnknown = true;
			line.m_isCounterReady = false;
			line.m_lastLightBarrierBits = 0;
			line.m_lastEjectionControlBit = true;
			line.m_sendCounterValue = 0;

			line.OnInstruction(
						CTracerMessages::SetMode::Id,
						instructionBuffer,
						instructionBufferSize,
						responseBuffer,
						responseBufferSize,
						responseSize);
		}

		break;

	case CMultiTracerMessages::SingleTrigger::Id:
		if (		(instructionBufferSize >= sizeof(CMultiTracerMessages::SingleTrigger)) &&
					(responseBufferSize >= sizeof(CMultiTracerMessages::SingleTrigger::Result))){
			const CMultiTracerMessages::SingleTrigger& instruction = *(const CMultiTracerMessages::SingleTrigger*)instructionBuffer;

			int lineIndex = int(instruction.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				line.OnInstruction(
							CTracerMessages::SingleTrigger::Id,
							&instruction.line,
							sizeof(CMultiTracerMessages::SingleTrigger::Result),
							responseBuffer,
							responseBufferSize,
							responseSize);
			}
		}
		break;

	case CMultiTracerMessages::GetLineInfo::Id:
		if (		(instructionBufferSize >= sizeof(CMultiTracerMessages::GetLineInfo)) &&
					(responseBufferSize >= sizeof(CMultiTracerMessages::GetLineInfo::Result))){
			const CMultiTracerMessages::GetLineInfo& instruction = *(const CMultiTracerMessages::GetLineInfo*)instructionBuffer;
			CMultiTracerMessages::GetLineInfo::Result& result = *(CMultiTracerMessages::GetLineInfo::Result*)responseBuffer;

			int lineIndex = int(instruction.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				line.OnInstruction(
							CTracerMessages::GetLineInfo::Id,
							NULL,
							0,
							responseBuffer,
							responseBufferSize,
							responseSize);

				int stationsCount = line.GetStationsCount();
				I_DWORD sensorBitsMask = I_DWORD((1 << stationsCount) - 1);
				I_DWORD sensorBits = (m_inputBits >> line.m_lightBarrierBitsIndex) & sensorBitsMask;
				bool ejectionControlBit = ((m_inputBits >> line.m_ejectionControlBitIndex) & 1) != 0;

				result.sensorBits = sensorBits;
				result.ejectionControlBit = ejectionControlBit? 1: 0;

				responseSize = sizeof(CMultiTracerMessages::GetLineInfo::Result);
			}
		}

		break;

	case CMultiTracerMessages::PopId::Id:
		if (instructionBufferSize >= sizeof(CMultiTracerMessages::PopId)){
			const CMultiTracerMessages::PopId& instruction = *(const CMultiTracerMessages::PopId*)instructionBuffer;

			int lineIndex = int(instruction.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				line.OnInstruction(
							CTracerMessages::PopId::Id,
							&instruction.line,
							sizeof(CTracerMessages::PopId),
							responseBuffer,
							responseBufferSize,
							responseSize);
			}
		}

		break;

	case CMultiTracerMessages::SetResult::Id:
		if (instructionBufferSize >= sizeof(CMultiTracerMessages::SetResult)){
			const CMultiTracerMessages::SetResult& instruction = *(const CMultiTracerMessages::SetResult*)instructionBuffer;

			int lineIndex = int(instruction.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				line.OnInstruction(
							CTracerMessages::SetResult::Id,
							&instruction.line,
							sizeof(CTracerMessages::PopId),
							responseBuffer,
							responseBufferSize,
							responseSize);
			}
		}

		break;

	default:
		return BaseClass::OnInstruction(
					instructionCode,
					instructionBuffer,
					instructionBufferSize,
					responseBuffer,
					responseBufferSize,
					responseSize);
	}

	return true;
}


void CMultiTracerDriverBase::OnHardwareInterrupt(I_DWORD interruptFlags)
{
	for (int lineIndex = 0; lineIndex < m_params.linesCount; ++lineIndex){
		SingleLine& line = m_lines[lineIndex];

		int stationsCount = line.GetStationsCount();

		I_DWORD maskedInputBits = m_inputBits & m_interruptsMask;

		I_DWORD barrierBitsMask = I_DWORD((1 << stationsCount) - 1);
		I_DWORD barrierBits = (maskedInputBits >> line.m_lightBarrierBitsIndex) & barrierBitsMask;

		I_DWORD barrierEdgeBits = (barrierBits ^ line.m_lastLightBarrierBits) & barrierBits;
		line.m_lastLightBarrierBits = barrierBits;

		if (barrierEdgeBits != 0){
			line.OnLightBarrierEdge(barrierEdgeBits);
		}

		bool ejectionControlBit = ((maskedInputBits >> line.m_ejectionControlBitIndex) & 1) != 0;

		line.m_lastEjectionControlBit = ejectionControlBit;

		line.OnHardwareInterrupt(line.m_isCounterReady?
					interruptFlags | CIoCardTracerDriverBase::IF_ENCODER_INTERRUPT:
					interruptFlags);
		I_ASSERT(!line.m_isCounterReady || (line.m_sendCounterValue > 0));	// if counter was ready, new value should be set
	}
}


void CMultiTracerDriverBase::OnPulse()
{
	for (int lineIndex = 0; lineIndex < m_params.linesCount; ++lineIndex){
		SingleLine& line = m_lines[lineIndex];

		line.OnPulse();
	}
}


// protected methods

void CMultiTracerDriverBase::ReadHardwareValues(__int64 microsecsTimer, SingleLine::NativeTimer internalTimer)
{
	I_ASSERT(internalTimer != 0);
	I_ASSERT(m_nativeTimer == 0);

	m_microsecsTimer = microsecsTimer;
	m_nativeTimer = internalTimer;

	m_inputBits = ReadPort();

	for (int lineIndex = 0; lineIndex < m_params.linesCount; ++lineIndex){
		SingleLine& line = m_lines[lineIndex];

		I_ASSERT(line.m_sendCounterValue == 0);

		I_WORD counterValue = ReadCounter(lineIndex);
		bool isCounterReadyBit = (((m_inputBits >> line.m_counterReadyBitIndex) & 1) != 0);
		line.m_isCounterReady = isCounterReadyBit;

		// correction of counter value
		if (line.m_isCounterReadyUnknown){
			if (line.m_isCounterReady){
				line.m_isCounterReady = false;
			}
			else{
				line.m_isCounterReadyUnknown = false;
			}
		}

		I_WORD counterDiff = line.m_lastCounterReadValue - counterValue;

		if (!line.m_isCounterUnknown || (counterDiff >= 2) || line.m_isCounterReady){
			line.m_isCounterUnknown = false;
			line.m_isCounterReadyUnknown = false;
			line.m_isCounterReady = isCounterReadyBit;

			line.UpdateLinePosition(counterValue);
		}
	}
}


void CMultiTracerDriverBase::WriteHardwareValues()
{
	for (int lineIndex = 0; lineIndex < m_params.linesCount; ++lineIndex){
		SingleLine& line = m_lines[lineIndex];

		if (line.m_sendCounterValue > 0){
			WriteCounter(lineIndex, line.m_sendCounterValue);

			line.m_lastCounterReadValue = ReadCounter(lineIndex);
			line.m_isCounterUnknown = true;
			line.m_isCounterReadyUnknown = true;
			line.m_isCounterReady = false;

			line.m_sendCounterValue = 0;
		}
	}

	if (m_outputBits != m_lastOutputBits){
		WritePort(m_outputBits);

		m_lastOutputBits = m_outputBits;
	}

	m_nativeTimer = 0;
}


void CMultiTracerDriverBase::SetOutputBits(I_DWORD value, I_DWORD mask)
{
	I_ASSERT((value & (~mask)) == 0);	// no bits outside mask are set

	m_outputBits = (m_outputBits & ~mask) | value;
}


void CMultiTracerDriverBase::CalcInterruptsMask()
{
	I_DWORD newInterruptsMask = 0;

	for (int lineIndex = 0; lineIndex < m_params.linesCount; ++lineIndex){
		SingleLine& line = m_lines[lineIndex];

		if (line.m_lightBarrierBitsIndex >= 0){
			int stationsCount = line.GetStationsCount();
			I_DWORD mask = ((1 << stationsCount) - 1) << line.m_lightBarrierBitsIndex;
			newInterruptsMask |= mask;
		}

		if (line.m_ejectionControlBitIndex >= 0){
			I_DWORD mask = 1 << line.m_ejectionControlBitIndex;
			newInterruptsMask |= mask;
		}

		if (line.m_counterReadyBitIndex >= 0){
			I_DWORD mask = (1 << line.m_counterReadyBitIndex);
			newInterruptsMask |= mask;
		}
	}

	if (newInterruptsMask != m_interruptsMask){
		WriteInterruptsMask(newInterruptsMask);

		m_interruptsMask = newInterruptsMask;
	}
}


void CMultiTracerDriverBase::ResetQueueLine(int lineIndex)
{
	SingleLine& line = m_lines[lineIndex];

	WriteCounter(lineIndex, 0);
	line.m_lastCounterReadValue = ReadCounter(lineIndex);
	line.m_isCounterUnknown = true;
	line.m_isCounterReadyUnknown = true;
	line.m_isCounterReady = false;
	line.m_lastLightBarrierBits = 0;
	line.m_lastEjectionControlBit = true;
	line.m_sendCounterValue = 0;

	line.ResetQueue();
}


// public methods of embedded class SingleLine

CMultiTracerDriverBase::SingleLine::SingleLine()
{
	m_lightBarrierBitsIndex = 1;
	m_ejectionControlBitIndex = -1;
	m_counterReadyBitIndex = -1;
	m_triggersBitIndex = -1;
	m_ejectorsBitIndex = -1;
	m_iosBitIndex = -1;

	Init(NULL);
}


void CMultiTracerDriverBase::SingleLine::Init(CMultiTracerDriverBase* parentPtr)
{
	m_parentPtr = parentPtr;

	m_lastLightBarrierBits = 0;
	m_lastEjectionControlBit = true;
	m_sendCounterValue = 0;
}


// reimplemented (ilolv::CTracerDriverBase)

void CMultiTracerDriverBase::SingleLine::SetTriggerBit(int bit, bool state)
{
	I_ASSERT(m_parentPtr != NULL);

	if ((m_parentPtr != NULL) && (m_triggersBitIndex != -1)){
		I_ASSERT(bit < GetStationsCount());

		I_DWORD mask = I_DWORD(1 << (m_triggersBitIndex + bit));

		m_parentPtr->SetOutputBits((state? mask: 0), mask);
	}
}


void CMultiTracerDriverBase::SingleLine::SetEjectorBit(int ejectorIndex, bool state)
{
	I_ASSERT(m_parentPtr != NULL);
	I_ASSERT(ejectorIndex >= 0);
	I_ASSERT(ejectorIndex < GetEjectorsCount());

	if ((m_parentPtr != NULL) && (m_ejectorsBitIndex != -1)){
		I_DWORD mask = I_DWORD(1 << (m_ejectorsBitIndex + ejectorIndex));

		m_parentPtr->SetOutputBits(state? mask: 0, mask);
	}
}


void CMultiTracerDriverBase::SingleLine::SetIoBit(int bitIndex, bool state)
{
	I_ASSERT(m_parentPtr != NULL);
	I_ASSERT(bitIndex >= 0);
	I_ASSERT(bitIndex <= IB_LAST);

	if ((m_parentPtr != NULL) && (m_iosBitIndex != -1)){
		I_DWORD mask = I_DWORD(1 << (m_iosBitIndex + bitIndex));

		m_parentPtr->SetOutputBits(state? mask: 0, mask);
	}
}


__int64 CMultiTracerDriverBase::SingleLine::GetCurrentTimer() const
{
	I_ASSERT(m_parentPtr != NULL);

	if (m_parentPtr != NULL){
		return m_parentPtr->GetCurrentTimer();
	}
	else{
		return 0;
	}
}


CMultiTracerDriverBase::SingleLine::NativeTimer CMultiTracerDriverBase::SingleLine::GetCurrentNativeTimer() const
{
	I_ASSERT(m_parentPtr != NULL);

	if (m_parentPtr != NULL){
		return m_parentPtr->GetCurrentNativeTimer();
	}
	else{
		return 0;
	}
}


bool CMultiTracerDriverBase::SingleLine::GetEjectionControlBit() const
{
	return m_lastEjectionControlBit;
}


void CMultiTracerDriverBase::SingleLine::SendMessage(int category, int id, const char* errorTxt, int* valuesPtr, int paramsCount)
{
	if (m_parentPtr != NULL){
		m_parentPtr->SendMessage(category, id, errorTxt, valuesPtr, paramsCount);
	}
}


// protected methods of embedded class SingleLine

// reimplemented (ilolv::CIoCardTracerDriverBase)

void CMultiTracerDriverBase::SingleLine::SetEncoderCounter(I_WORD value)
{
	I_ASSERT(I_SWORD(value) > 0);
	
	m_sendCounterValue = value;
}


} // namespace ilolv


