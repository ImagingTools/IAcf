#include "ilolv/CSignalBitsDriverBase.h"


#include "ilolv/CGeneralInfoCommands.h"


namespace ilolv
{


CSignalBitsDriverBase::CSignalBitsDriverBase()
:	m_lastHeartbeatTime(0),
	m_doHeartbeatPullDown(false),
	m_applicationStatus(CGeneralInfoCommands::MC_CRITICAL),
	m_shownStatus(CGeneralInfoCommands::MC_CRITICAL)
{
	m_params.heartbeatPeriod = 10000;
	m_params.signalBitsCount = 0;

	for (int signalIndex = 0; signalIndex < MAX_SIGNALS; ++signalIndex){
		m_signalBitIndices[signalIndex] = -1;
	}
}


// reimplemented (ilolv::IDriver)

bool CSignalBitsDriverBase::OnCommand(
			I_DWORD commandCode,
			const void* commandBuffer,
			int commandBufferSize,
			void* /*responseBuffer*/,
			int /*responseBufferSize*/,
			I_DWORD& responseSize)
{
	I_ASSERT((commandBuffer != NULL) || (commandBufferSize == 0));

	responseSize = 0;

	switch (commandCode){
	case CSignalBitsCommands::SetParams::Id:
		if (commandBufferSize >= sizeof(CSignalBitsCommands::SetParams)){
			m_params = *(const CSignalBitsCommands::SetParams*)commandBuffer;
			if (m_params.signalBitsCount > MAX_SIGNALS){
				m_params.signalBitsCount = MAX_SIGNALS;
			}
		}
		break;

	case CSignalBitsCommands::SetSignalBitIndex::Id:
		if (commandBufferSize >= sizeof(CSignalBitsCommands::SetSignalBitIndex)){
			const CSignalBitsCommands::SetSignalBitIndex& command = *(const CSignalBitsCommands::SetSignalBitIndex*)commandBuffer;

			if ((command.signalIndex >= 0) && (command.signalIndex < MAX_SIGNALS)){
				m_signalBitIndices[command.signalIndex] = command.bitIndex;
			}
		}
		break;

	case CSignalBitsCommands::SetApplicationStatus::Id:
		if (commandBufferSize >= sizeof(CSignalBitsCommands::SetApplicationStatus)){
			const CSignalBitsCommands::SetApplicationStatus& command = *(const CSignalBitsCommands::SetApplicationStatus*)commandBuffer;

			m_applicationStatus = command.status;
		}

		break;
	}

	return true;
}


void CSignalBitsDriverBase::OnHardwareInterrupt(I_DWORD interruptFlags)
{
	if ((interruptFlags & IF_PULSE_TIMER) != 0){
		__int64 currentTimer = GetCurrentTimer();

		if (m_applicationStatus < CGeneralInfoCommands::MC_CRITICAL){
			if (m_doHeartbeatPullDown){
				if (currentTimer > m_lastHeartbeatTime + (m_params.heartbeatPeriod / 2)){
					m_doHeartbeatPullDown = false;
					SetSignalBit(CSignalBitsCommands::SB_HEARTBEAT, false);
				}
			}
			else if (currentTimer > m_lastHeartbeatTime + m_params.heartbeatPeriod){
				m_lastHeartbeatTime = currentTimer;
				m_doHeartbeatPullDown = true;
				SetSignalBit(CSignalBitsCommands::SB_HEARTBEAT, true);
			}
		}

		if (m_applicationStatus != m_shownStatus){
			bool wasErrorBit = (m_shownStatus >= CGeneralInfoCommands::MC_ERROR);
			bool isErrorBit = (m_applicationStatus >= CGeneralInfoCommands::MC_ERROR);
			if (wasErrorBit != isErrorBit){
				SetSignalBit(CSignalBitsCommands::SB_ERROR, isErrorBit);
			}

			bool wasWarningBit = (m_shownStatus >= CGeneralInfoCommands::MC_WARNING);
			bool isWarningBit = (m_applicationStatus >= CGeneralInfoCommands::MC_WARNING);
			if (wasWarningBit != isWarningBit){
				SetSignalBit(CSignalBitsCommands::SB_WARNING, isWarningBit);
			}

			m_shownStatus = m_applicationStatus;
		}
	}
}


// protected methods

void CSignalBitsDriverBase::SetSignalBit(int signal, bool state)
{
	if (		(signal >= CSignalBitsCommands::SB_HEARTBEAT) &&
				(signal <= m_params.signalBitsCount)){
		int index = m_signalBitIndices[signal];
		if (index >= 0){
			I_DWORD mask = I_DWORD(1 << index);
			SetOutputBits(state? 0: mask, mask);
		}
	}
}


} // namespace ilolv


