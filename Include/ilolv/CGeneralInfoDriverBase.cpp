#include "ilolv/CGeneralInfoDriverBase.h"


namespace ilolv
{


CGeneralInfoDriverBase::CGeneralInfoDriverBase()
:	m_nextMinKeepAliveTime(0)
{
	m_params.maxKeepAliveTime = 0;
	m_message.category = -1;
}


// reimplemented (ilolv::IDriver)

bool CGeneralInfoDriverBase::OnInstruction(
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
	case CGeneralInfoMessages::SetParams::Id:
		if (instructionBufferSize >= sizeof(CGeneralInfoMessages::SetParams)){
			m_params = *(const CGeneralInfoMessages::SetParams*)instructionBuffer;
		}
		break;

	case CGeneralInfoMessages::PopMessage::Id:
		if (		(instructionBufferSize >= sizeof(CGeneralInfoMessages::PopMessage)) &&
					(responseBufferSize >= sizeof(CGeneralInfoMessages::PopMessage::Result))){
			OnPopMessageInstruction(*(CGeneralInfoMessages::PopMessage::Result*)responseBuffer);

			responseSize = sizeof(CGeneralInfoMessages::PopMessage::Result);
		}
		break;

	case CGeneralInfoMessages::KeepAlive::Id:
		OnKeepAlive();
		break;

	default:
		return false;
	}

	return true;
}


void CGeneralInfoDriverBase::OnHardwareInterrupt(I_DWORD /*interruptFlags*/)
{
}


void CGeneralInfoDriverBase::OnPeriodicPulse()
{
	__int64 currentTimer = GetCurrentTimer();
	if (		(m_nextMinKeepAliveTime > 0) &&
				(currentTimer > m_nextMinKeepAliveTime)){
		SendMessage(CGeneralInfoMessages::MC_CRITICAL,
					CGeneralInfoMessages::MI_NO_RESPONSE,
					"Application doesn't response");

		m_nextMinKeepAliveTime = 0;
	}
}


// protected methods

void CGeneralInfoDriverBase::SendMessage(int category, int id, const char* errorTxt, int* valuesPtr, int paramsCount)
{
	if (category > m_message.category){
		m_message.category = category;
		m_message.id = id;

		for (int i = 0; i < CGeneralInfoMessages::MAX_ERROR_MESSAGE_SIZE; ++i){
			char c = errorTxt[i];
			m_message.text[i] = c;

			if (c == '\0'){
				break;
			}
		}

		I_ASSERT(paramsCount <= CGeneralInfoMessages::MAX_MESSAGE_PARAMS_COUNT);
		if (paramsCount > CGeneralInfoMessages::MAX_MESSAGE_PARAMS_COUNT){
			paramsCount = CGeneralInfoMessages::MAX_MESSAGE_PARAMS_COUNT;
		}

		for (int i = 0; i < paramsCount; ++i){
			m_message.params[i] = valuesPtr[i];
		}

		m_message.paramsCount = paramsCount;
	}
}


// private methods

void CGeneralInfoDriverBase::OnPopMessageInstruction(CGeneralInfoMessages::PopMessage::Result& result)
{
	result.category = m_message.category;

	if (m_message.category >= 0){
		m_message.category = -1;

		result.id = m_message.id;
		const char* errPtr = m_message.text;
		I_ASSERT(errPtr != NULL);

		int i = 0;
		for (; (i < CGeneralInfoMessages::MAX_ERROR_MESSAGE_SIZE - 1) && (errPtr[i] != '\0'); ++i){
			result.message[i] = errPtr[i];
		}

		result.message[i] = '\0';

		int paramsCount = m_message.paramsCount;
		I_ASSERT(paramsCount <= CGeneralInfoMessages::MAX_MESSAGE_PARAMS_COUNT);

		if (paramsCount > CGeneralInfoMessages::MAX_MESSAGE_PARAMS_COUNT){
			paramsCount = CGeneralInfoMessages::MAX_MESSAGE_PARAMS_COUNT;
		}

		for (int i = 0; i < paramsCount; ++i){
			result.params[i] = I_SDWORD(m_message.params[i]);
		}

		result.paramsCount = I_SDWORD(paramsCount);
	}
}


void CGeneralInfoDriverBase::OnKeepAlive()
{
	if (m_params.maxKeepAliveTime > 0){
		m_nextMinKeepAliveTime = GetCurrentTimer() + m_params.maxKeepAliveTime;
	}
	else{
		m_nextMinKeepAliveTime = 0;
	}
}


} // namespace ilolv


