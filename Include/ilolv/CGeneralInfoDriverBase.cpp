#include "ilolv/CGeneralInfoDriverBase.h"


namespace ilolv
{


CGeneralInfoDriverBase::CGeneralInfoDriverBase()
:	m_nextMinKeepAliveTime(0)
{
	m_messagePosition = 0;

	m_params.maxKeepAliveTime = 0;
	m_message.category = -1;
}


// reimplemented (ilolv::IDriver)

bool CGeneralInfoDriverBase::OnCommand(
			int commandCode,
			const void* commandBuffer,
			int commandBufferSize,
			void* responseBuffer,
			int responseBufferSize,
			int& responseSize)
{
	I_ASSERT((commandBuffer != NULL) || (commandBufferSize == 0));

	responseSize = 0;

	switch (commandCode){
	case CGeneralInfoCommands::SetParams::Id:
		if (commandBufferSize >= int(sizeof(CGeneralInfoCommands::SetParams))){
			m_params = *(const CGeneralInfoCommands::SetParams*)commandBuffer;
		}
		break;

	case CGeneralInfoCommands::PopMessage::Id:
		if (		(commandBufferSize >= int(sizeof(CGeneralInfoCommands::PopMessage))) &&
					(responseBufferSize >= int(sizeof(CGeneralInfoCommands::PopMessage::Result)))){
			OnPopMessageCommand(*(CGeneralInfoCommands::PopMessage::Result*)responseBuffer);

			responseSize = int(sizeof(CGeneralInfoCommands::PopMessage::Result));
		}
		break;

	case CGeneralInfoCommands::KeepAlive::Id:
		OnKeepAliveCommand();
		break;

	default:
		return false;
	}

	return true;
}


void CGeneralInfoDriverBase::OnHardwareInterrupt(I_DWORD interruptFlags)
{
	if ((interruptFlags & IF_PULSE_TIMER) != 0){
		I_SQWORD currentTimer = GetCurrentTimer();
		if (		(m_nextMinKeepAliveTime > 0) &&
					(currentTimer > m_nextMinKeepAliveTime)){
			AppendMessage(CGeneralInfoCommands::MC_CRITICAL,
						CGeneralInfoCommands::MI_NO_RESPONSE,
						"Application doesn't response",
						true);

			m_nextMinKeepAliveTime = 0;
		}
	}
}


// protected methods

// reimplemented (ilolv::IDriver)

void CGeneralInfoDriverBase::AppendMessage(int category, int id, const char* text, bool doSend)
{
	if ((m_messagePosition > 0) || (category > m_message.category)){
		if (m_messagePosition <= 0){
			m_message.category = category;
			m_message.id = id;
		}

		while (m_messagePosition < CGeneralInfoCommands::MAX_ERROR_MESSAGE_SIZE){
			char c = *(text++);
			m_message.text[m_messagePosition] = c;

			if (c == '\0'){
				break;
			}

			m_messagePosition++;
		}
	}

	if (doSend){
		m_messagePosition = 0;
	}
}


// private methods

void CGeneralInfoDriverBase::OnPopMessageCommand(CGeneralInfoCommands::PopMessage::Result& result)
{
	result.category = m_message.category;

	if (m_message.category >= 0){
		m_message.category = -1;
		m_messagePosition = 0;

		result.id = m_message.id;
		const char* text = m_message.text;
		I_ASSERT(text != NULL);

		int i = 0;
		for (; (i < CGeneralInfoCommands::MAX_ERROR_MESSAGE_SIZE - 1) && (text[i] != '\0'); ++i){
			result.text[i] = text[i];
		}

		result.text[i] = '\0';
	}
}


void CGeneralInfoDriverBase::OnKeepAliveCommand()
{
	if (m_params.maxKeepAliveTime > 0){
		m_nextMinKeepAliveTime = GetCurrentTimer() + m_params.maxKeepAliveTime;
	}
	else{
		m_nextMinKeepAliveTime = 0;
	}
}


} // namespace ilolv


