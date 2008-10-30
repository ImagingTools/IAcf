#include "ilolv/CIoCardTracerDriverBase.h"


#include "ilolv/CGeneralInfoCommands.h"


namespace ilolv
{


CIoCardTracerDriverBase::CIoCardTracerDriverBase()
{
	m_nextEventPosition = 0x10000;
	m_linePosition = 0;

	m_ioParams.lightBarriersBitIndex = 1;
	m_ioParams.ejectorsBitIndex = 1;
	m_ioParams.triggersBitIndex = 1;

	m_isPositionEventActive = false;
}


I_DWORD CIoCardTracerDriverBase::GetInterruptsMask() const
{
	I_DWORD retVal = 0;

	if (m_ioParams.lightBarriersBitIndex >= 0){
		int unitsCount = GetTracerParams().lightBarriersCount;

		for (int i = 0; i < unitsCount; ++i){
			const CInspectionUnitCommands::UnitParams& unitParams = GetUnitParams(i);

			if (unitParams.lightBarrierIndex >= 0){
				I_DWORD unitMask = (1 << (m_ioParams.lightBarriersBitIndex + unitParams.lightBarrierIndex));

				retVal |= unitMask;
			}
		}
	}

	return retVal;
}


// reimplemented (ilolv::CTracerDriverBase)

I_DWORD CIoCardTracerDriverBase::GetLinePosition() const
{
	return m_linePosition;
}


bool CIoCardTracerDriverBase::GetLightBarrierBit(int lightBarrierIndex) const
{
	I_ASSERT(lightBarrierIndex >= 0);
	I_ASSERT(lightBarrierIndex < GetTracerParams().lightBarriersCount);

	int bitIndex = m_ioParams.lightBarriersBitIndex + lightBarrierIndex;

	return ((m_inputBits >> bitIndex) & 1) != 0;
}


void CIoCardTracerDriverBase::SetTriggerBit(int bit, bool state)
{
	I_ASSERT(bit >= 0);
	I_ASSERT(bit < GetTracerParams().ejectorsCount);

	if (m_ioParams.triggersBitIndex != -1){
		I_DWORD mask = I_DWORD(1 << (m_ioParams.triggersBitIndex + bit));

		SetOutputBits((state? mask: 0), mask);
	}
}


void CIoCardTracerDriverBase::SetEjectorBit(int ejectorIndex, bool state)
{
	I_ASSERT(ejectorIndex >= 0);
	I_ASSERT(ejectorIndex < GetTracerParams().ejectorsCount);

	if (m_ioParams.ejectorsBitIndex != -1){
		I_DWORD mask = I_DWORD(1 << (m_ioParams.ejectorsBitIndex + ejectorIndex));

		SetOutputBits(state? mask: 0, mask);
	}
}


__int64 CIoCardTracerDriverBase::GetCurrentTimer() const
{
	return m_currentTimer;
}


IDriver::NativeTimer CIoCardTracerDriverBase::GetCurrentNativeTimer() const
{
	return m_currentNativeTimer;
}


// reimplemented (ilolv::IDriver)

bool CIoCardTracerDriverBase::OnCommand(
			int commandCode,
			const void* commandBuffer,
			int commandBufferSize,
			void* responseBuffer,
			int responseBufferSize,
			int& responseSize)
{
	responseSize = 0;

	switch (commandCode){
	case CIoCardTracerCommands::SetIoParams::Id:
		if (commandBufferSize >= sizeof(CIoCardTracerCommands::SetIoParams)){
			m_ioParams = *(const CIoCardTracerCommands::SetIoParams*)commandBuffer;
		}
		break;

	case CIoCardTracerCommands::GetLightBarrierInfo::Id:
		if (		(commandBufferSize >= sizeof(CIoCardTracerCommands::GetLightBarrierInfo)) &&
					(responseBufferSize >= sizeof(CIoCardTracerCommands::GetLightBarrierInfo::Result))){
			const CIoCardTracerCommands::GetLightBarrierInfo& command = *(const CIoCardTracerCommands::GetLightBarrierInfo*)commandBuffer;
			CIoCardTracerCommands::GetLightBarrierInfo::Result& result = *(CIoCardTracerCommands::GetLightBarrierInfo::Result*)responseBuffer;

			result.state = GetLightBarrierBit(command.lightBarrierIndex);

			responseSize = sizeof(CIoCardTracerCommands::GetLightBarrierInfo::Result);
		}
		break;

	default:
		return BaseClass::OnCommand(
					commandCode,
					commandBuffer, commandBufferSize,
					responseBuffer, responseBufferSize,
					responseSize);
	}

	return true;
}


void CIoCardTracerDriverBase::OnHardwareInterrupt(I_DWORD interruptFlags)
{
	BaseClass::OnHardwareInterrupt(interruptFlags);

	if ((interruptFlags & IF_ENCODER_INTERRUPT) != 0){
		OnCounterReady();
	}
}


// reimplemented (ilolv::IDigitalIo)

I_DWORD CIoCardTracerDriverBase::GetInputBits() const
{
	return m_inputBits;
}


// protected methods

void CIoCardTracerDriverBase::UpdateHardwareValues(I_DWORD inputBits, I_WORD counterValue, __int64 microsecsTimer, IDriver::NativeTimer nativeTimer)
{
	m_inputBits = inputBits;
	m_linePosition += I_SWORD(m_lastCounterValue - counterValue);
	m_currentTimer = microsecsTimer;
	m_currentNativeTimer = nativeTimer;
	m_lastCounterValue = counterValue;
}


void CIoCardTracerDriverBase::OnCounterReady()
{
	if (m_isPositionEventActive){
		I_SDWORD difference = I_SDWORD(m_nextEventPosition - GetLinePosition());
		if (difference <= 0){
			m_isPositionEventActive = false;

			OnSinglePositionEvent();

			return;
		}
		else if (difference < MAX_COUNTER_VALUE){
			SetEncoderCounter(I_WORD(difference));

			return;
		}
	}

	SetEncoderCounter(MAX_COUNTER_VALUE);
}


// reimplemented (ilolv::CSingleCountTracerDriverBase)

void CIoCardTracerDriverBase::SetNextSinglePositionEvent(const I_DWORD* eventPositionPtr)
{
	if (eventPositionPtr != NULL){
		m_nextEventPosition = *eventPositionPtr;
		m_isPositionEventActive = true;

	}
	else{
		m_isPositionEventActive = false;
	}

	CIoCardTracerDriverBase::OnCounterReady();
}


} // namespace ilolv


