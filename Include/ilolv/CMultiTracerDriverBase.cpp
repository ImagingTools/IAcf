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
		m_lines[lineIndex].Init(lineIndex, this);
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

	case CMultiTracerMessages::SetLineIoParams::Id:
		if (instructionBufferSize >= sizeof(CMultiTracerMessages::SetLineIoParams)){
			const CMultiTracerMessages::SetLineIoParams& instruction = *(const CMultiTracerMessages::SetLineIoParams*)instructionBuffer;

			int lineIndex = int(instruction.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				line.m_counterReadyBitIndex = instruction.readyBitIndex;

				line.OnInstruction(
							CIoCardTracerMessages::SetIoParams::Id,
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
		if (instructionBufferSize >= sizeof(CMultiTracerMessages::GetLineInfo)){
			const CMultiTracerMessages::GetLineInfo& instruction = *(const CMultiTracerMessages::GetLineInfo*)instructionBuffer;

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
			}
		}

		break;

	case CMultiTracerMessages::GetLineLightBarrierInfo::Id:
		if (instructionBufferSize >= sizeof(CMultiTracerMessages::GetLineLightBarrierInfo)){
			const CMultiTracerMessages::GetLineLightBarrierInfo& instruction = *(const CMultiTracerMessages::GetLineLightBarrierInfo*)instructionBuffer;

			int lineIndex = int(instruction.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				return line.OnInstruction(
							CIoCardTracerMessages::GetLightBarrierInfo::Id,
							NULL,
							0,
							responseBuffer,
							responseBufferSize,
							responseSize);
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
		return false;
	}

	return true;
}


void CMultiTracerDriverBase::OnHardwareInterrupt(I_DWORD interruptFlags)
{
	for (int lineIndex = 0; lineIndex < m_params.linesCount; ++lineIndex){
		SingleLine& line = m_lines[lineIndex];

		line.OnHardwareInterrupt(
					line.m_isCounterReady?
					interruptFlags | CIoCardTracerDriverBase::IF_ENCODER_INTERRUPT:
					interruptFlags);
		I_ASSERT(!line.m_isCounterReady || (line.m_sendCounterValue > 0));	// if counter was ready, new value should be set
	}
}


void CMultiTracerDriverBase::OnPeriodicPulse()
{
	for (int lineIndex = 0; lineIndex < m_params.linesCount; ++lineIndex){
		SingleLine& line = m_lines[lineIndex];

		line.OnPeriodicPulse();
	}
}


// reimplemented (ilolv::IDigitalIo)

void CMultiTracerDriverBase::SetOutputBits(I_DWORD value, I_DWORD mask)
{
	I_ASSERT((value & (~mask)) == 0);	// no bits outside mask are set

	m_outputBits = (m_outputBits & ~mask) | value;
}


// protected methods

void CMultiTracerDriverBase::ReadHardwareValues(__int64 microsecsTimer, IDriver::NativeTimer internalTimer)
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

			line.UpdateHardwareValues(m_inputBits, I_DWORD(I_SDWORD(I_SWORD(counterValue))), m_microsecsTimer, m_nativeTimer);
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


void CMultiTracerDriverBase::CalcInterruptsMask()
{
	I_DWORD newInterruptsMask = 0;

	for (int lineIndex = 0; lineIndex < m_params.linesCount; ++lineIndex){
		SingleLine& line = m_lines[lineIndex];

		newInterruptsMask |= line.GetInterruptsMask();
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
	line.m_sendCounterValue = 0;

	line.ResetQueue();
}


// public methods of embedded class SingleLine

CMultiTracerDriverBase::SingleLine::SingleLine()
{
	m_counterReadyBitIndex = -1;

	Init(-1, NULL);
}


void CMultiTracerDriverBase::SingleLine::Init(int lineNumber, CMultiTracerDriverBase* parentPtr)
{
	m_lineNumber = lineNumber;
	m_parentPtr = parentPtr;

	m_sendCounterValue = 0;
}


I_DWORD CMultiTracerDriverBase::SingleLine::GetInterruptsMask() const
{
	I_DWORD retVal = BaseClass::GetInterruptsMask();

	if (m_counterReadyBitIndex >= 0){
		I_DWORD mask = (1 << m_counterReadyBitIndex);

		retVal |= mask;
	}

	return retVal;
}


// reimplemented (ilolv::IDigitalIo)

void CMultiTracerDriverBase::SingleLine::SetOutputBits(I_DWORD value, I_DWORD mask)
{
	if (m_parentPtr != NULL){
		m_parentPtr->SetOutputBits(value, mask);
	}
}


// protected methods of embedded class SingleLine

// reimplemented (ilolv::CIoCardTracerDriverBase)

void CMultiTracerDriverBase::SingleLine::SetEncoderCounter(I_WORD value)
{
	I_ASSERT(I_SWORD(value) > 0);
	
	m_sendCounterValue = value;
}


// reimplemented (ilolv::IDriver)

void CMultiTracerDriverBase::SingleLine::AppendMessage(int category, int id, const char* text, bool doSend)
{
	if (m_parentPtr != NULL){
		m_parentPtr->AppendMessage(category, id, text, false);

		if (doSend){
			m_parentPtr->AppendMessage(category, id, ": Line ", false);
			char numberText[2];
			numberText[0] = '1' + char(m_lineNumber);
			numberText[1] = '\0';
			m_parentPtr->AppendMessage(category, id, numberText, true);
		}
	}
}


} // namespace ilolv


