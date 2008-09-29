#ifndef ilolv_CIoCardTracerDriverBase_included
#define ilolv_CIoCardTracerDriverBase_included


#include "ilolv/CSingleCountTracerDriverBase.h"


namespace ilolv
{


/**
	Implementation of line controller based on simple standard I/O card hardware.
 */
class CIoCardTracerDriverBase: public CSingleCountTracerDriverBase
{
public:
	typedef CSingleCountTracerDriverBase BaseClass;

	enum InterruptFlags
	{
		IF_ENCODER_INTERRUPT = 0x200
	};

	CIoCardTracerDriverBase();

	// reimplemented (ilolv::CTracerDriverBase)
	virtual I_DWORD GetLinePosition() const;

	// reimplemented (ilolv::IDriver)
	virtual void OnHardwareInterrupt(I_DWORD interruptFlags);

protected:
	bool TrySetNextEventFromFifo();
	/**	Updates line position to new counter value.
	 *		It should be called before any hardware event
	 *		(OnCounterReady, OnLightBarrierEdge or OnPeriodicPulse) is called.
	 */
	void UpdateLinePosition(I_WORD counterValue);
	/**	Set position of event counter.
	 */
	void SetEventCounterValue(I_WORD value);

	void OnCounterReady();

	// reimplemented (ilolv::CSingleCountTracerDriverBase)
	virtual void SetNextSinglePositionEvent(const I_DWORD* eventPositionPtr);

	// abstract methods
	/**
		Set counter value to specified value.
		This counter counts encoder pulses down.
		When 0 is achived, implementation of counter has to call method \c OnHardwareInterrupt with set flag IF_ENCODER_INTERRUPT.
	*/
	virtual void SetEncoderCounter(I_WORD value) = 0;

private:
	enum
	{
		MAX_COUNTER_VALUE = 0x7ff8
	};

	I_DWORD m_counterPosition;

	I_DWORD m_nextEventPosition;
	bool m_isPositionEventActive;
};


} // namespace ilolv


#endif // !ilolv_CIoCardTracerDriverBase_included


