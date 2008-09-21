#include "ilolv/CIoCardTracerDriverBase.h"


#include "ilolv/CGeneralInfoMessages.h"


namespace ilolv
{


CIoCardTracerDriverBase::CIoCardTracerDriverBase()
{
	m_nextEventPosition = 0x10000;
	m_counterPosition = 0;

	m_positionEventsCount = 0;
	m_idleCyclesCount = -1;

	for (int fifoIndex = 0; fifoIndex < MAX_EVENTS_COUNT; ++fifoIndex){
		EventsFifo& fifo = eventsFifos[fifoIndex];
		fifo.nextIndex = 0;
		fifo.lastIndex = 0;

		for (int i = 0; i < EVENTS_FIFO_SIZE; ++i){
			fifo.positions[i] = 0;
		}
	}
}


void CIoCardTracerDriverBase::ResetQueue()
{
	for (int fifoIndex = 0; fifoIndex < m_positionEventsCount; ++fifoIndex){
		EventsFifo& fifo = eventsFifos[fifoIndex];
		fifo.nextIndex = 0;
		fifo.lastIndex = 0;
	}

	BaseClass::ResetQueue();

//	m_counterPosition = 0;
//	m_nextEventPosition = m_counterPosition + 0x10000;

	SetIdleEventPosition();
}


// reimplemented (ilolv::CTracerDriverBase)

bool CIoCardTracerDriverBase::SetCounterQueuesCount(int count)
{
	if (count <= MAX_POSITION_EVENTS_COUNT){
		m_positionEventsCount = count;

		return true;
	}
	else{
		m_positionEventsCount = MAX_POSITION_EVENTS_COUNT;

		return false;
	}
}


void CIoCardTracerDriverBase::InsertPositionToQueue(int queueIndex, I_DWORD counterPosition)
{
	I_ASSERT(queueIndex >= 0);
	I_ASSERT(queueIndex < m_positionEventsCount);

	if (I_SWORD(counterPosition - m_counterPosition) <= 0){
		OnPositionEvent(queueIndex);

		return;
	}

	EventsFifo& fifo = eventsFifos[queueIndex];

	bool wasFifoEmpty = (fifo.lastIndex == fifo.nextIndex);

	int fifoIndex = fifo.nextIndex;
	int nextFifoIndex = CalcNextEventsFifoIndex(fifoIndex);

	if (nextFifoIndex == fifo.lastIndex){	// if this event queue is empty
		int lineIndex = GetLineIndex() + 1;
		SendMessage(CGeneralInfoMessages::MC_CRITICAL,
					CTracerMessages::MI_QUEUE_ERROR,
					"Line%1:EventFifoIsFull",
					&lineIndex,
					1);

		return;
	}

	fifo.positions[fifoIndex] = counterPosition;
	fifo.nextIndex = nextFifoIndex;

	if (wasFifoEmpty){	// if was empty, check if event should be redirected
		TrySetNextEventFromFifo();
	}
}


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

void CIoCardTracerDriverBase::SetNextEventPosition(I_DWORD eventPosition)
{
	I_SDWORD distance = eventPosition - m_counterPosition;

	if (distance > 0){
		if (distance <= IDLE_COUNT){
			m_idleCyclesCount = 0;

			SetEventCounterValue(I_WORD(distance));
		}
		else{
			m_idleCyclesCount = distance % IDLE_COUNT;	// late event mode

			SetEventCounterValue(IDLE_COUNT);
		}
	}
	else{
		m_idleCyclesCount = 0;

		OnCounterReady();
	}
}


void CIoCardTracerDriverBase::SetIdleEventPosition()
{
	SetEventCounterValue(IDLE_COUNT);

	m_idleCyclesCount = -1;	// idle mode
}


void CIoCardTracerDriverBase::OnCounterReady()
{
	if (m_idleCyclesCount > 0){
		if (--m_idleCyclesCount > 0){
			SetEventCounterValue(IDLE_COUNT);
		}
		else{
			TrySetNextEventFromFifo();
		}
	}
	else if (m_idleCyclesCount < 0){
		SetEventCounterValue(IDLE_COUNT);
	}
	else{
		for (int eventIndex = m_positionEventsCount - 1; eventIndex >= 0; --eventIndex){	// backwards iteration, becouse of events priority
			EventsFifo& fifo = eventsFifos[eventIndex];
			if (fifo.lastIndex != fifo.nextIndex){
				I_DWORD position = fifo.positions[fifo.lastIndex];
				I_SDWORD diff(position - m_counterPosition);
				if (diff <= 0){
					fifo.lastIndex = CalcNextFifoIndex(fifo.lastIndex);

					OnPositionEvent(eventIndex);
				}
			}
		}

		TrySetNextEventFromFifo();
	}
}


bool CIoCardTracerDriverBase::TrySetNextEventFromFifo()
{
	int nextFifoIndex = -1;
	I_SDWORD bestPosDiff = 0x7fffffff;
	for (int index = 0; index < m_positionEventsCount; ++index){
		const EventsFifo& fifo = eventsFifos[index];
		if (fifo.lastIndex != fifo.nextIndex){
			I_DWORD position = fifo.positions[fifo.lastIndex];
			I_SDWORD posDiff = position - m_counterPosition;

			if (posDiff < bestPosDiff){
				bestPosDiff = posDiff;
				nextFifoIndex = index;
			}
		}
	}

	if (nextFifoIndex >= 0){
		const EventsFifo& fifo = eventsFifos[nextFifoIndex];
		I_DWORD position = fifo.positions[fifo.lastIndex];

		SetNextEventPosition(position);

		return true;
	}
	else{
		SetIdleEventPosition();
	}

	return false;
}


void CIoCardTracerDriverBase::UpdateLinePosition(I_WORD counterValue)
{
	I_DWORD newLinePosition = I_DWORD(m_nextEventPosition - I_SDWORD(I_SWORD(counterValue)));

	I_ASSERT(I_SDWORD(newLinePosition - m_counterPosition) >= 0);

	m_counterPosition = newLinePosition;
}


void CIoCardTracerDriverBase::SetEventCounterValue(I_WORD value)
{
	I_ASSERT(value > 0);
	I_ASSERT(value <= IDLE_COUNT);


	I_DWORD newEventPosition = m_counterPosition + value;
	if (m_nextEventPosition != newEventPosition){
		m_nextEventPosition = newEventPosition;
		SetEncoderCounter(value);
	}
}


} // namespace ilolv


