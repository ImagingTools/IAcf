#include "ilolv/CSingleCountTracerDriverBase.h"


#include "ilolv/CGeneralInfoCommands.h"


namespace ilolv
{


CSingleCountTracerDriverBase::CSingleCountTracerDriverBase()
{
	m_positionEventsCount = 0;

	for (int fifoIndex = 0; fifoIndex < MAX_EVENTS_COUNT; ++fifoIndex){
		EventQueue& eventQueue = m_positionEventQueues[fifoIndex];

		eventQueue.Reset();
	}
}


// reimplemented (ilolv::CTracerDriverBase)

void CSingleCountTracerDriverBase::ResetQueue()
{
	for (int fifoIndex = 0; fifoIndex < m_positionEventsCount; ++fifoIndex){
		EventQueue& eventQueue = m_positionEventQueues[fifoIndex];

		eventQueue.Reset();
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


void CSingleCountTracerDriverBase::InsertPositionToQueue(int queueIndex, I_DWORD counterPosition, void* userContext)
{
	I_ASSERT(queueIndex >= 0);
	I_ASSERT(queueIndex < m_positionEventsCount);

	I_DWORD currentLinePosition = GetLinePosition();
	if (I_SWORD(counterPosition - currentLinePosition) <= 0){
		ProcessPositionEvent(queueIndex, userContext);

		return;
	}

	EventQueue& eventQueue = m_positionEventQueues[queueIndex];

	bool wasFifoEmpty = eventQueue.IsEmpty();

	if (eventQueue.Insert(counterPosition, &userContext) >= 0){
		if (wasFifoEmpty){	// if was empty, check if event should be redirected
			CalculateNextSinglePositionEvent();
		}
	}
	else{
		AppendMessage(CGeneralInfoCommands::MC_CRITICAL,
					CTracerCommands::MI_QUEUE_ERROR,
					"Cannot insert position to event queue",
					true);

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
		const EventQueue& eventQueue = m_positionEventQueues[index];
		if (!eventQueue.IsEmpty()){
			I_DWORD position = eventQueue.GetBackPosition();
			I_SDWORD posDiff = position - currentLinePosition;

			if (posDiff < bestPosDiff){
				bestPosDiff = posDiff;
				nextFifoIndex = index;
			}
		}
	}

	if (nextFifoIndex >= 0){
		const EventQueue& eventQueue = m_positionEventQueues[nextFifoIndex];

		I_ASSERT(!eventQueue.IsEmpty());
		I_DWORD position = eventQueue.GetBackPosition();

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
		EventQueue& eventQueue = m_positionEventQueues[eventIndex];

		while (!eventQueue.IsEmpty() && (I_SDWORD(eventQueue.GetBackPosition() - currentLinePosition) <= 0)){
			void* userContext = eventQueue.GetBackObject();

			ProcessPositionEvent(eventIndex, userContext);

			eventQueue.PopBack();
		}
	}

	CalculateNextSinglePositionEvent();
}


} // namespace ilolv


