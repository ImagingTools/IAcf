#include "ilolv/CSignalBitsDriverBase.h"


#include "ilolv/CGeneralInfoMessages.h"


namespace ilolv
{


CSignalBitsDriverBase::CSignalBitsDriverBase()
:	m_lastHeartbeatTime(0),
	m_doHeartbeatPullDown(false),
	m_applicationStatus(CGeneralInfoMessages::MC_CRITICAL),
	m_shownStatus(CGeneralInfoMessages::MC_CRITICAL)
{
	m_params.heartbeatPeriod = 10000;
	m_params.signalBitsCount = 0;

	for (int signalIndex = 0; signalIndex < MAX_SIGNALS; ++signalIndex){
		m_signalBitIndices[signalIndex] = -1;
	}
}


// reimplemented (ilolv::IDriver)

bool CSignalBitsDriverBase::OnInstruction(
			I_DWORD instructionCode,
			const void* instructionBuffer,
			int instructionBufferSize,
			void* /*responseBuffer*/,
			int /*responseBufferSize*/,
			I_DWORD& responseSize)
{
	I_ASSERT((instructionBuffer != NULL) || (instructionBufferSize == 0));

	responseSize = 0;

	switch (instructionCode){
	case CSignalBitsMessages::SetParams::Id:
		if (instructionBufferSize >= sizeof(CSignalBitsMessages::SetParams)){
			m_params = *(const CSignalBitsMessages::SetParams*)instructionBuffer;
			if (m_params.signalBitsCount > MAX_SIGNALS){
				m_params.signalBitsCount = MAX_SIGNALS;
			}
		}
		break;

	case CSignalBitsMessages::SetSignalBitIndex::Id:
		if (instructionBufferSize >= sizeof(CSignalBitsMessages::SetSignalBitIndex)){
			const CSignalBitsMessages::SetSignalBitIndex& instruction = *(const CSignalBitsMessages::SetSignalBitIndex*)instructionBuffer;

			if ((instruction.signalIndex >= 0) && (instruction.signalIndex < MAX_SIGNALS)){
				m_signalBitIndices[instruction.signalIndex] = instruction.bitIndex;
			}
		}
		break;

	case CSignalBitsMessages::SetApplicationStatus::Id:
		if (instructionBufferSize >= sizeof(CSignalBitsMessages::SetApplicationStatus)){
			const CSignalBitsMessages::SetApplicationStatus& instruction = *(const CSignalBitsMessages::SetApplicationStatus*)instructionBuffer;

			m_applicationStatus = instruction.status;
		}

		break;
	}

	return true;
}


void CSignalBitsDriverBase::OnHardwareInterrupt(I_DWORD /*interruptFlags*/)
{
}


void CSignalBitsDriverBase::OnPeriodicPulse()
{
	__int64 currentTimer = GetCurrentTimer();

	if (m_applicationStatus < CGeneralInfoMessages::MC_CRITICAL){
		if (m_doHeartbeatPullDown){
			if (currentTimer > m_lastHeartbeatTime + (m_params.heartbeatPeriod / 2)){
				m_doHeartbeatPullDown = false;
				SetSignalBit(CSignalBitsMessages::SB_HEARTBEAT, false);
			}
		}
		else if (currentTimer > m_lastHeartbeatTime + m_params.heartbeatPeriod){
			m_lastHeartbeatTime = currentTimer;
			m_doHeartbeatPullDown = true;
			SetSignalBit(CSignalBitsMessages::SB_HEARTBEAT, true);
		}
	}

	if (m_applicationStatus != m_shownStatus){
		bool wasErrorBit = (m_shownStatus >= CGeneralInfoMessages::MC_ERROR);
		bool isErrorBit = (m_applicationStatus >= CGeneralInfoMessages::MC_ERROR);
		if (wasErrorBit != isErrorBit){
			SetSignalBit(CSignalBitsMessages::SB_ERROR, isErrorBit);
		}

		bool wasWarningBit = (m_shownStatus >= CGeneralInfoMessages::MC_WARNING);
		bool isWarningBit = (m_applicationStatus >= CGeneralInfoMessages::MC_WARNING);
		if (wasWarningBit != isWarningBit){
			SetSignalBit(CSignalBitsMessages::SB_WARNING, isWarningBit);
		}

		m_shownStatus = m_applicationStatus;
	}
}


// protected methods

void CSignalBitsDriverBase::SetSignalBit(int signal, bool state)
{
	if (		(signal >= CSignalBitsMessages::SB_HEARTBEAT) &&
				(signal <= m_params.signalBitsCount)){
		int index = m_signalBitIndices[signal];
		if (index >= 0){
			I_DWORD mask = I_DWORD(1 << index);
			SetOutputBits(state? 0: mask, mask);
		}
	}
}


} // namespace ilolv


