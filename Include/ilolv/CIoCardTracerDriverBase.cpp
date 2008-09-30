#include "ilolv/CIoCardTracerDriverBase.h"


#include "ilolv/CGeneralInfoMessages.h"


namespace ilolv
{


CIoCardTracerDriverBase::CIoCardTracerDriverBase()
{
	m_nextEventPosition = 0x10000;
	m_counterPosition = 0;

	m_isPositionEventActive = false;
}


// reimplemented (ilolv::CTracerDriverBase)

I_DWORD CIoCardTracerDriverBase::GetLinePosition() const
{
	return m_counterPosition;
}


// reimplemented (ilolv::IDriver)

void CIoCardTracerDriverBase::OnHardwareInterrupt(I_DWORD interruptFlags)
{
	if ((interruptFlags & IF_ENCODER_INTERRUPT) != 0){
		OnCounterReady();
	}
}


// protected methods

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


void CIoCardTracerDriverBase::UpdateLinePosition(I_WORD counterValue)
{
	I_DWORD newLinePosition = I_DWORD(m_nextEventPosition - I_SDWORD(I_SWORD(counterValue)));

	I_ASSERT(I_SDWORD(newLinePosition - m_counterPosition) >= 0);

	m_counterPosition = newLinePosition;
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


