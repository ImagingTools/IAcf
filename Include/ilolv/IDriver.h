#ifndef ilolv_IDriver_included
#define ilolv_IDriver_included


#include "ilolv.h"


namespace ilolv
{


class IDriver
{
public:
	typedef __int64 NativeTimer;

	virtual ~IDriver(){}

	/**
		Called if user instruction should be executed.
		\param	instructionCode		code of instruction.
		\param	instructionBuffer	pointer to instruction buffer.
		\param	instructionBufferSize		size of instruction buffer in bytes.
		\param	responseBuffer		pointer to response buffer.
		\param	responseSize		size of response buffer in bytes.
									It is in/out value, should be set to real size of response.
		\return	true if this instruction was consumed.
	*/
	virtual bool OnInstruction(
				I_DWORD instructionCode,
				const void* instructionBuffer,
				int instructionBufferSize,
				void* responseBuffer,
				int responseBufferSize,
				I_DWORD& responseSize) = 0;

	/**
		Called if hardware interrupt is occured.
		\param	interruptFlags	interrupt source flags.
	*/
	virtual void OnHardwareInterrupt(I_DWORD interruptFlags) = 0;

	/**
		Called in regular period to provide time controlled events.
	*/
	virtual void OnPeriodicPulse() = 0;

protected:
	/**
		Get value of timer in microseconds.
	*/
	virtual __int64 GetCurrentTimer() const = 0;
	/**
		Get native timer.
		Native timer is needed to exchange absolute timer value between driver and application.
	*/
	virtual NativeTimer GetCurrentNativeTimer() const = 0;
};


} // namespace ilolv


#endif // !ilolv_IDriver_included


