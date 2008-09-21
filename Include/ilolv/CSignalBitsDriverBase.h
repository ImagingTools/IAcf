#ifndef ilolv_CSignalBitsDriverBase_included
#define ilolv_CSignalBitsDriverBase_included


#include "ilolv/IDriver.h"
#include "ilolv/CSignalBitsMessages.h"


namespace ilolv
{


/**	Base class for implementation of production driver on the driver side.
 */
class CSignalBitsDriverBase: public IDriver
{
public:
	CSignalBitsDriverBase();

	// reimplemented (ilolv::IDriver)
	virtual bool OnInstruction(
				I_DWORD instructionCode,
				const void* instructionBuffer,
				int instructionBufferSize,
				void* responseBuffer,
				int responseBufferSize,
				I_DWORD& responseSize);
	virtual void OnHardwareInterrupt(I_DWORD interruptFlags);
	virtual void OnPulse();

protected:
	enum{
		MAX_SIGNALS = 16
	};

	/**
		Set signal bit to specified state.
	*/
	void SetSignalBit(int signal, bool state);

	// abstract methods
	virtual void SetOutputBits(I_DWORD value, I_DWORD mask) = 0;

private:
	CSignalBitsMessages::SignalParams m_params;

	// indices of output bits
	int m_signalBitIndices[MAX_SIGNALS];

	int m_shownStatus;
	int m_applicationStatus;

	// internal counters
	__int64 m_lastHeartbeatTime;
	bool m_doHeartbeatPullDown;
};


} // namespace ilolv


#endif // !ilolv_CSignalBitsDriverBase_included


