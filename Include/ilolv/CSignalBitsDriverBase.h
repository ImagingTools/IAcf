#ifndef ilolv_CSignalBitsDriverBase_included
#define ilolv_CSignalBitsDriverBase_included


#include "ilolv/IDriver.h"
#include "ilolv/IDigitalIo.h"
#include "ilolv/CSignalBitsCommands.h"


namespace ilolv
{


/**
	Implementation of standard bits signalizing (like error, warning and heartbeat output bits) for drivers.
*/
class CSignalBitsDriverBase: virtual public IDriver, virtual public IDigitalIo
{
public:
	CSignalBitsDriverBase();

	const CSignalBitsCommands::SignalParams& GetSignalParams() const;

	// reimplemented (ilolv::IDriver)
	virtual bool OnCommand(
				I_DWORD commandCode,
				const void* commandBuffer,
				int commandBufferSize,
				void* responseBuffer,
				int responseBufferSize,
				I_DWORD& responseSize);
	virtual void OnHardwareInterrupt(I_DWORD interruptFlags);

protected:
	enum{
		MAX_SIGNALS = 16
	};

	/**
		Set signal bit to specified state.
	*/
	void SetSignalBit(int signal, bool state);

private:
	CSignalBitsCommands::SignalParams m_params;

	// indices of output bits
	int m_signalBitIndices[MAX_SIGNALS];

	int m_shownStatus;
	int m_applicationStatus;

	// internal counters
	__int64 m_lastHeartbeatTime;
	bool m_doHeartbeatPullDown;
};


// inline methods

inline const CSignalBitsCommands::SignalParams& CSignalBitsDriverBase::GetSignalParams() const
{
	return m_params;
}


} // namespace ilolv


#endif // !ilolv_CSignalBitsDriverBase_included


