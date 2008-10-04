#include "ilolv/CMultiTracerDriverBase.h"


#include "ilolv/CGeneralInfoCommands.h"


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

bool CMultiTracerDriverBase::OnCommand(
			I_DWORD commandCode,
			const void* commandBuffer,
			int commandBufferSize,
			void* responseBuffer,
			int responseBufferSize,
			I_DWORD& responseSize)
{
	I_ASSERT((commandBuffer != NULL) || (commandBufferSize == 0));

	responseSize = 0;

	switch (commandCode){
	case CMultiTracerCommands::SetParams::Id:
		if (commandBufferSize >= sizeof(CMultiTracerCommands::SetParams)){
			m_params = *(const CMultiTracerCommands::SetParams*)commandBuffer;
			if (m_params.linesCount > MAX_LINES){
				m_params.linesCount = MAX_LINES;
			}

			CalcInterruptsMask();
		}
		break;

	case CMultiTracerCommands::SetLineParams::Id:
		if (commandBufferSize >= sizeof(CMultiTracerCommands::SetLineParams)){
			const CMultiTracerCommands::SetLineParams& command = *(const CMultiTracerCommands::SetLineParams*)commandBuffer;

			int lineIndex = int(command.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				line.OnCommand(
							CTracerCommands::SetParams::Id,
							&command.line,
							sizeof(CMultiTracerCommands::SetLineParams),
							responseBuffer,
							responseBufferSize,
							responseSize);

				CalcInterruptsMask();
			}
		}
		break;

	case CMultiTracerCommands::SetLineIoParams::Id:
		if (commandBufferSize >= sizeof(CMultiTracerCommands::SetLineIoParams)){
			const CMultiTracerCommands::SetLineIoParams& command = *(const CMultiTracerCommands::SetLineIoParams*)commandBuffer;

			int lineIndex = int(command.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				line.m_counterReadyBitIndex = command.readyBitIndex;

				line.OnCommand(
							CIoCardTracerCommands::SetIoParams::Id,
							&command.line,
							sizeof(CMultiTracerCommands::SetLineParams),
							responseBuffer,
							responseBufferSize,
							responseSize);

				CalcInterruptsMask();
			}
		}
		break;

	case CMultiTracerCommands::SetUnitParams::Id:
		if (commandBufferSize >= sizeof(CMultiTracerCommands::SetUnitParams)){
			const CMultiTracerCommands::SetUnitParams& command = *(const CMultiTracerCommands::SetUnitParams*)commandBuffer;

			int lineIndex = int(command.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				line.OnCommand(
							CTracerCommands::SetUnitParams::Id,
							&command.line,
							sizeof(CMultiTracerCommands::SetUnitParams),
							responseBuffer,
							responseBufferSize,
							responseSize);
			}
		}
		break;

	case CMultiTracerCommands::SetEjectorParams::Id:
		if (commandBufferSize >= sizeof(CMultiTracerCommands::SetEjectorParams)){
			const CMultiTracerCommands::SetEjectorParams& command = *(const CMultiTracerCommands::SetEjectorParams*)commandBuffer;

			int lineIndex = int(command.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				line.OnCommand(
							CTracerCommands::SetEjectorParams::Id,
							&command.line,
							sizeof(CMultiTracerCommands::SetEjectorParams),
							responseBuffer,
							responseBufferSize,
							responseSize);
			}
		}
		break;

	case CMultiTracerCommands::SetMode::Id:
		for (int lineIndex = 0; lineIndex < MAX_LINES; ++lineIndex){
			SingleLine& line = m_lines[lineIndex];

			WriteCounter(lineIndex, 0);
			line.m_lastCounterReadValue = ReadCounter(lineIndex);
			line.m_isCounterUnknown = true;
			line.m_isCounterReadyUnknown = true;
			line.m_isCounterReady = false;
			line.m_lastEjectionControlBit = true;
			line.m_sendCounterValue = 0;

			line.OnCommand(
						CTracerCommands::SetMode::Id,
						commandBuffer,
						commandBufferSize,
						responseBuffer,
						responseBufferSize,
						responseSize);
		}

		break;

	case CMultiTracerCommands::SingleTrigger::Id:
		if (		(commandBufferSize >= sizeof(CMultiTracerCommands::SingleTrigger)) &&
					(responseBufferSize >= sizeof(CMultiTracerCommands::SingleTrigger::Result))){
			const CMultiTracerCommands::SingleTrigger& command = *(const CMultiTracerCommands::SingleTrigger*)commandBuffer;

			int lineIndex = int(command.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				line.OnCommand(
							CTracerCommands::SingleTrigger::Id,
							&command.line,
							sizeof(CMultiTracerCommands::SingleTrigger::Result),
							responseBuffer,
							responseBufferSize,
							responseSize);
			}
		}
		break;

	case CMultiTracerCommands::GetLineInfo::Id:
		if (commandBufferSize >= sizeof(CMultiTracerCommands::GetLineInfo)){
			const CMultiTracerCommands::GetLineInfo& command = *(const CMultiTracerCommands::GetLineInfo*)commandBuffer;

			int lineIndex = int(command.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				line.OnCommand(
							CTracerCommands::GetLineInfo::Id,
							NULL,
							0,
							responseBuffer,
							responseBufferSize,
							responseSize);
			}
		}

		break;

	case CMultiTracerCommands::GetLineLightBarrierInfo::Id:
		if (commandBufferSize >= sizeof(CMultiTracerCommands::GetLineLightBarrierInfo)){
			const CMultiTracerCommands::GetLineLightBarrierInfo& command = *(const CMultiTracerCommands::GetLineLightBarrierInfo*)commandBuffer;

			int lineIndex = int(command.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				return line.OnCommand(
							CIoCardTracerCommands::GetLightBarrierInfo::Id,
							NULL,
							0,
							responseBuffer,
							responseBufferSize,
							responseSize);
			}
		}

		break;

	case CMultiTracerCommands::PopId::Id:
		if (commandBufferSize >= sizeof(CMultiTracerCommands::PopId)){
			const CMultiTracerCommands::PopId& command = *(const CMultiTracerCommands::PopId*)commandBuffer;

			int lineIndex = int(command.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				line.OnCommand(
							CTracerCommands::PopId::Id,
							&command.line,
							sizeof(CTracerCommands::PopId),
							responseBuffer,
							responseBufferSize,
							responseSize);
			}
		}

		break;

	case CMultiTracerCommands::SetResult::Id:
		if (commandBufferSize >= sizeof(CMultiTracerCommands::SetResult)){
			const CMultiTracerCommands::SetResult& command = *(const CMultiTracerCommands::SetResult*)commandBuffer;

			int lineIndex = int(command.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				line.OnCommand(
							CTracerCommands::SetResult::Id,
							&command.line,
							sizeof(CTracerCommands::PopId),
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


