#ifndef ilolv_CSingleCountTracerDriverBase_included
#define ilolv_CSingleCountTracerDriverBase_included


#include "ilolv/CTracerDriverBase.h"
#include "ilolv/COrderedPositionsQueue.h"


namespace ilolv
{


/**
	Object tracer based on single line position counter.
	This single counter will be 'multiplied' using set of queues.
*/
class CSingleCountTracerDriverBase: public CTracerDriverBase
{
public:
	typedef CTracerDriverBase BaseClass;

	CSingleCountTracerDriverBase();

	// reimplemented (ilolv::CTracerDriverBase)
	virtual void ResetQueue();
	virtual bool SetCounterQueuesCount(int count);
	virtual void InsertPositionToQueue(int queueIndex, I_DWORD counterPosition);

protected:
	/**
		Check all position queues and calculate the next single position event.
	*/
	bool CalculateNextSinglePositionEvent();

	/**
		Called by derived implementation when single position event is occured.
	*/
	void OnSinglePositionEvent();

	// abstract methods
	/**
		Set or remove position of next position event.
		\param	eventPositionPtr	optional position value.
									If this is NULL, position of next event will be removed.
	*/
	virtual void SetNextSinglePositionEvent(const I_DWORD* eventPositionPtr) = 0;

private:
	enum
	{
		MAX_EVENTS_COUNT = 50,
	};

	COrderedPositionsQueue eventsFifos[MAX_EVENTS_COUNT];
	int m_positionEventsCount;
};


} // namespace ilolv


#endif // !ilolv_CSingleCountTracerDriverBase_included


