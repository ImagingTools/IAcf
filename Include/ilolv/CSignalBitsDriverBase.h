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
				int commandCode,
				const void* commandBuffer,
				int commandBufferSize,
				void* responseBuffer,
				int responseBufferSize,
				int& responseSize);
	virtual void OnHardwareInterrupt(I_DWORD interruptFlags);

protected:
	enum{
		MAX_SIGNALS = 16,
		MAX_SIGNAL_BITS = 16
	};

	/**
		Set signal bit to specified state.
	*/
	void SetSignalBit(int signal, bool state);

	// reimplemented (ilolv::IDriver)
	virtual int CreateSignalId();
	virtual bool SetSignalState(int signalId, int state);

private:
	CSignalBitsCommands::SignalParams m_params;

	// indices of output bits
	int m_signalBitIndices[MAX_SIGNAL_BITS];

	int m_signalStates[MAX_SIGNALS];
	int m_nextFreeSignalId;

	int m_globalStatus;
	int m_shownGlobalStatus;

	// internal counters
	I_SQWORD m_lastHeartbeatTime;
	bool m_doHeartbeatPullDown;
};


// inline methods

inline const CSignalBitsCommands::SignalParams& CSignalBitsDriverBase::GetSignalParams() const
{
	return m_params;
}


} // namespace ilolv


#endif // !ilolv_CSignalBitsDriverBase_included


