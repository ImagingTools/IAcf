#include "ilolv/CSingleCountTracerDriverBase.h"


#include "ilolv/CGeneralInfoMessages.h"


namespace ilolv
{


CSingleCountTracerDriverBase::CSingleCountTracerDriverBase()
{
	m_positionEventsCount = 0;

	for (int fifoIndex = 0; fifoIndex < MAX_EVENTS_COUNT; ++fifoIndex){
		COrderedPositionsQueue& fifo = eventsFifos[fifoIndex];

		fifo.Reset();
	}
}


// reimplemented (ilolv::CTracerDriverBase)

void CSingleCountTracerDriverBase::ResetQueue()
{
	for (int fifoIndex = 0; fifoIndex < m_positionEventsCount; ++fifoIndex){
		COrderedPositionsQueue& fifo = eventsFifos[fifoIndex];

		fifo.Reset();
	}

	BaseClass::ResetQueue();

	SetNextSinglePositionEvent(NULL);
}


bool CSingleCountTracerDriverBase::SetCounterQueuesCount(int count)
{
	if (count <= MAX_EVENTS_COUNT){
		m_positionEventsCount = count;

		return true;
	}
	else{
		m_positionEventsCount = MAX_EVENTS_COUNT;

		return false;
	}
}


void CSingleCountTracerDriverBase::InsertPositionToQueue(int queueIndex, I_DWORD counterPosition)
{
	I_ASSERT(queueIndex >= 0);
	I_ASSERT(queueIndex < m_positionEventsCount);

	I_DWORD currentLinePosition = GetLinePosition();
	if (I_SWORD(counterPosition - currentLinePosition) <= 0){
		OnPositionEvent(queueIndex);

		return;
	}

	COrderedPositionsQueue& fifo = eventsFifos[queueIndex];

	bool wasFifoEmpty = fifo.IsEmpty();

	if (fifo.Insert(counterPosition)){
		if (wasFifoEmpty){	// if was empty, check if event should be redirected
			CalculateNextSinglePositionEvent();
		}
	}
	else{
		int lineIndex = GetLineIndex() + 1;

		SendMessage(CGeneralInfoMessages::MC_CRITICAL,
					CTracerMessages::MI_QUEUE_ERROR,
					"Line%1:CannotInsertPosition",
					&lineIndex,
					1);

		return;
	}
}


// protected methods

bool CSingleCountTracerDriverBase::CalculateNextSinglePositionEvent()
{
	I_DWORD currentLinePosition = GetLinePosition();

	int nextFifoIndex = -1;
	I_SDWORD bestPosDiff = 0x7fffffff;
	for (int index = 0; index < m_positionEventsCount; ++index){
		const COrderedPositionsQueue& fifo = eventsFifos[index];
		if (!fifo.IsEmpty()){
			I_DWORD position = fifo.GetFront();
			I_SDWORD posDiff = position - currentLinePosition;

			if (posDiff < bestPosDiff){
				bestPosDiff = posDiff;
				nextFifoIndex = index;
			}
		}
	}

	if (nextFifoIndex >= 0){
		const COrderedPositionsQueue& fifo = eventsFifos[nextFifoIndex];

		I_ASSERT(!fifo.IsEmpty());
		I_DWORD position = fifo.GetFront();

		SetNextSinglePositionEvent(&position);

		return true;
	}
	else{
		SetNextSinglePositionEvent(NULL);

		return false;
	}
}


void CSingleCountTracerDriverBase::OnSinglePositionEvent()
{
	I_DWORD currentLinePosition = GetLinePosition();

	for (int eventIndex = m_positionEventsCount - 1; eventIndex >= 0; --eventIndex){
		COrderedPositionsQueue& fifo = eventsFifos[eventIndex];

		if (fifo.PopFrontTill(currentLinePosition)){
			OnPositionEvent(eventIndex);
		}
	}

	CalculateNextSinglePositionEvent();
}


} // namespace ilolv


