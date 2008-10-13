#include "ilolv/CMultiTracerDriverBase.h"


#include "ilolv/CGeneralInfoCommands.h"


namespace ilolv
{


CMultiTracerDriverBase::CMultiTracerDriverBase()
:	m_interruptsMask(0)
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

	case CMultiTracerCommands::SetLightBarrierParams::Id:
		if (commandBufferSize >= sizeof(CMultiTracerCommands::SetLightBarrierParams)){
			const CMultiTracerCommands::SetLightBarrierParams& command = *(const CMultiTracerCommands::SetLightBarrierParams*)commandBuffer;

			int lineIndex = int(command.lineIndex);
			if ((lineIndex >= 0) && (lineIndex < m_params.linesCount)){
				SingleLine& line = m_lines[lineIndex];

				line.OnCommand(
							CTracerCommands::SetLightBarrierParams::Id,
							&command.line,
							sizeof(CMultiTracerCommands::SetLightBarrierParams),
							responseBuffer,
							responseBufferSize,
							responseSize);
			}
		}
		break;

	case CMultiTracerCommands::SetMode::Id:
		for (int lineIndex = 0; lineIndex < MAX_LINES; ++lineIndex){
			SingleLine& line = m_lines[lineIndex];

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

		line.OnHardwareInterrupt(interruptFlags);
	}
}


// protected methods

void CMultiTracerDriverBase::CopyFromHardware()
{
	I_DWORD inputBits = GetInputBits();

	for (int lineIndex = 0; lineIndex < m_params.linesCount; ++lineIndex){
		SingleLine& line = m_lines[lineIndex];

		I_WORD counterValue = ReadCounter(lineIndex);

		line.UpdateHardwareValues(inputBits, I_DWORD(I_SDWORD(I_SWORD(counterValue))), GetCurrentTimer(), GetCurrentNativeTimer());
	}
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


// public methods of embedded class SingleLine

CMultiTracerDriverBase::SingleLine::SingleLine()
{
	Init(-1, NULL);
}


void CMultiTracerDriverBase::SingleLine::Init(int lineNumber, CMultiTracerDriverBase* parentPtr)
{
	m_lineNumber = lineNumber;
	m_parentPtr = parentPtr;
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

	if (m_parentPtr != NULL){
		m_parentPtr->WriteCounter(m_lineNumber, value);
	}
}


// reimplemented (ilolv::CTracerDriverBase)

void CMultiTracerDriverBase::SingleLine::ResetQueue()
{
	SetEncoderCounter(0);

	BaseClass::ResetQueue();
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


