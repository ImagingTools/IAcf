#ifndef ilolv_CSignalBitsDriverBase_included
#define ilolv_CSignalBitsDriverBase_included


#include "ilolv/IDriver.h"
#include "ilolv/IDigitalIo.h"
#include "ilolv/CSignalBitsMessages.h"


namespace ilolv
{


/**	Base class for implementation of production driver on the driver side.
 */
class CSignalBitsDriverBase: virtual public IDriver, virtual public IDigitalIo
{
public:
	CSignalBitsDriverBase();

	const CSignalBitsMessages::SignalParams& GetSignalParams() const;

	// reimplemented (ilolv::IDriver)
	virtual bool OnInstruction(
				I_DWORD instructionCode,
				const void* instructionBuffer,
				int instructionBufferSize,
				void* responseBuffer,
				int responseBufferSize,
				I_DWORD& responseSize);
	virtual void OnHardwareInterrupt(I_DWORD interruptFlags);
	virtual void OnPeriodicPulse();

protected:
	enum{
		MAX_SIGNALS = 16
	};

	/**
		Set signal bit to specified state.
	*/
	void SetSignalBit(int signal, bool state);

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


// inline methods

inline const CSignalBitsMessages::SignalParams& CSignalBitsDriverBase::GetSignalParams() const
{
	return m_params;
}


} // namespace ilolv


#endif // !ilolv_CSignalBitsDriverBase_included


