#ifndef ilolv_CIoCardTracerDriverBase_included
#define ilolv_CIoCardTracerDriverBase_included


#include "ilolv/CTracerDriverBase.h"


namespace ilolv
{


/**
	Implementation of line controller based on simple standard I/O card hardware.
 */
class CIoCardTracerDriverBase: public CTracerDriverBase
{
public:
	typedef CTracerDriverBase BaseClass;

	enum InterruptFlags
	{
		IF_ENCODER_INTERRUPT = 0x200
	};

	CIoCardTracerDriverBase();

	// reimplemented (ilolv::CTracerDriverBase)
	virtual void ResetQueue();
	virtual bool SetCounterQueuesCount(int count);
	virtual void InsertPositionToQueue(int queueIndex, I_DWORD counterPosition);
	virtual I_DWORD GetLinePosition() const;

	// reimplemented (ilolv::IDriver)
	virtual void OnHardwareInterrupt(I_DWORD interruptFlags);

protected:
	void SetNextEventPosition(I_DWORD eventPosition);
	void SetIdleEventPosition();

	bool TrySetNextEventFromFifo();
	/**	Updates line position to new counter value.
	 *		It should be called before any hardware event
	 *		(OnCounterReady, OnLightBarrierEdge or OnPulse) is called.
	 */
	void UpdateLinePosition(I_WORD counterValue);
	/**	Set position of event counter.
	 */
	void SetEventCounterValue(I_WORD value);

	void OnCounterReady();

	// static methods
	static int CalcNextEventsFifoIndex(int fifoIndex);

	// abstract methods
	/**
		Set counter value to specified value.
		This counter counts encoder pulses down.
		When 0 is achived, implementation of counter has to call method \c OnHardwareInterrupt with set flag IF_ENCODER_INTERRUPT.
	*/
	virtual void SetEncoderCounter(I_WORD value) = 0;

private:
	enum{
		MAX_POSITION_EVENTS_COUNT = 100,
		EVENTS_FIFO_SIZE = 256,
		EVENTS_FIFO_INDEX_MASK = EVENTS_FIFO_SIZE - 1,
		MAX_EVENTS_COUNT = 32,
		IDLE_COUNT = 0x7ff0
	};

	struct EventsFifo{
		I_DWORD positions[EVENTS_FIFO_SIZE];
		int nextIndex;
		int lastIndex;
	};

	EventsFifo eventsFifos[MAX_EVENTS_COUNT];
	int m_positionEventsCount;

	I_DWORD m_counterPosition;

	I_DWORD m_nextEventPosition;

	int m_idleCyclesCount;
};


// static methods of embedded class SingleLine

inline int CIoCardTracerDriverBase::CalcNextEventsFifoIndex(int fifoIndex)
{
	return (fifoIndex + 1) % EVENTS_FIFO_INDEX_MASK;
}


} // namespace ilolv


#endif // !ilolv_CIoCardTracerDriverBase_included


