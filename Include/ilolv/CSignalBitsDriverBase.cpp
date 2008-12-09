#include "ilolv/CSignalBitsDriverBase.h"


#include "ilolv/CGeneralInfoCommands.h"


namespace ilolv
{


CSignalBitsDriverBase::CSignalBitsDriverBase()
:	m_nextFreeSignalId(1),
	m_globalStatus(CGeneralInfoCommands::MC_INFO),
	m_shownGlobalStatus(-1),
	m_lastHeartbeatTime(0),
	m_doHeartbeatPullDown(false)
{
	m_signalStates[0] = CGeneralInfoCommands::MC_INFO;

	m_params.heartbeatPeriod = 10000;
	m_params.signalBitsCount = 0;

	for (int signalIndex = 0; signalIndex < MAX_SIGNAL_BITS; ++signalIndex){
		m_signalBitIndices[signalIndex] = -1;
	}
}


// reimplemented (ilolv::IDriver)

bool CSignalBitsDriverBase::OnCommand(
			int commandCode,
			const void* commandBuffer,
			int commandBufferSize,
			void* /*responseBuffer*/,
			int /*responseBufferSize*/,
			int& responseSize)
{
	I_ASSERT((commandBuffer != NULL) || (commandBufferSize == 0));

	responseSize = 0;

	switch (commandCode){
	case CSignalBitsCommands::SetParams::Id:
		if (commandBufferSize >= int(sizeof(CSignalBitsCommands::SetParams))){
			m_params = *(const CSignalBitsCommands::SetParams*)commandBuffer;
			if (m_params.signalBitsCount > MAX_SIGNAL_BITS){
				m_params.signalBitsCount = MAX_SIGNAL_BITS;
			}
		}
		break;

	case CSignalBitsCommands::SetSignalBitIndex::Id:
		if (commandBufferSize >= int(sizeof(CSignalBitsCommands::SetSignalBitIndex))){
			const CSignalBitsCommands::SetSignalBitIndex& command = *(const CSignalBitsCommands::SetSignalBitIndex*)commandBuffer;

			if ((command.signalIndex >= 0) && (command.signalIndex < MAX_SIGNAL_BITS)){
				m_signalBitIndices[command.signalIndex] = command.bitIndex;
			}
		}
		break;

	case CSignalBitsCommands::SetApplicationStatus::Id:
		if (commandBufferSize >= int(sizeof(CSignalBitsCommands::SetApplicationStatus))){
			const CSignalBitsCommands::SetApplicationStatus& command = *(const CSignalBitsCommands::SetApplicationStatus*)commandBuffer;

			m_signalStates[0] = command.status;
		}

		break;
	}

	return true;
}


void CSignalBitsDriverBase::OnHardwareInterrupt(I_DWORD interruptFlags)
{
	if ((interruptFlags & IF_PULSE_TIMER) != 0){
		I_SQWORD currentTimer = GetCurrentTimer();

		if (m_globalStatus < CGeneralInfoCommands::MC_CRITICAL){
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

		if (m_globalStatus != m_shownGlobalStatus){
			bool wasErrorBit = (m_shownGlobalStatus >= CGeneralInfoCommands::MC_ERROR);
			bool isErrorBit = (m_globalStatus >= CGeneralInfoCommands::MC_ERROR);
			if (wasErrorBit != isErrorBit){
				SetSignalBit(CSignalBitsCommands::SB_ERROR, isErrorBit);
			}

			bool wasWarningBit = (m_shownGlobalStatus >= CGeneralInfoCommands::MC_WARNING);
			bool isWarningBit = (m_globalStatus >= CGeneralInfoCommands::MC_WARNING);
			if (wasWarningBit != isWarningBit){
				SetSignalBit(CSignalBitsCommands::SB_WARNING, isWarningBit);
			}

			m_shownGlobalStatus = m_globalStatus;
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
			SetOutputBits(state? mask: 0, mask);
		}
	}
}


// reimplemented (ilolv::IDriver)

int CSignalBitsDriverBase::CreateSignalId()
{
	I_ASSERT(m_nextFreeSignalId <= MAX_SIGNALS);

	if (m_nextFreeSignalId < MAX_SIGNALS){
		m_signalStates[m_nextFreeSignalId] = SS_OK;

		return m_nextFreeSignalId++;
	}

	return -1;
}


bool CSignalBitsDriverBase::SetSignalState(int signalId, int state)
{
	I_ASSERT(m_nextFreeSignalId <= MAX_SIGNALS);
	I_ASSERT(signalId >= 0);
	I_ASSERT(signalId < m_nextFreeSignalId);

	if (state != m_signalStates[signalId]){
		bool needRecalculation = (m_globalStatus == m_signalStates[signalId]) || (state > m_globalStatus);

		m_signalStates[signalId] = state;

		if (needRecalculation){
			m_globalStatus = SS_OK;

			for (int i = 0; i < m_nextFreeSignalId; ++i){
				if (m_signalStates[i] > m_globalStatus){
					m_globalStatus = m_signalStates[i];
				}
			}

			return m_globalStatus == signalId;
		}
	}

	return false;
}


} // namespace ilolv


