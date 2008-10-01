#ifndef ilolv_CMultiTracerDriverBase_included
#define ilolv_CMultiTracerDriverBase_included


#include "ilolv/IDriver.h"
#include "ilolv/CMultiTracerMessages.h"
#include "ilolv/CIoCardTracerDriverBase.h"


namespace ilolv
{


/**	Base class for implementation of production driver on the driver side.
 */
class CMultiTracerDriverBase: virtual public IDriver
{
public:
	CMultiTracerDriverBase();

	const CMultiTracerMessages::MultiTracerParams& GetMultiTracerParams() const;

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

	// reimplemented (ilolv::IDigitalIo)
	void SetOutputBits(I_DWORD value, I_DWORD mask);

protected:
	enum{
		MAX_LINES = 3,
		MAX_COUNTER_VALUE = 0x8000
	};

	class SingleLine: public ilolv::CIoCardTracerDriverBase
	{
	public:
		friend class CMultiTracerDriverBase;

		typedef ilolv::CIoCardTracerDriverBase BaseClass;

		SingleLine();

		void Init(int lineNumber, CMultiTracerDriverBase* parentPtr);

		I_DWORD GetInterruptsMask() const;

		// reimplemented (ilolv::IDigitalIo)
		virtual void SetOutputBits(I_DWORD value, I_DWORD mask);

	protected:
		// reimplemented (ilolv::CIoCardTracerDriverBase)
		virtual void SetEncoderCounter(I_WORD value);

		// reimplemented (ilolv::IDriver)
		virtual void AppendMessage(int category, int id, const char* text, bool doSend = true);

	private:
		// temporaly stored I/O hardware values
		bool m_lastEjectionControlBit;

		// temporaly stored counter hardware values
		I_WORD m_lastCounterReadValue;
		bool m_isCounterUnknown;
		bool m_isCounterReadyUnknown;
		I_WORD m_sendCounterValue;
		bool m_isCounterReady;

		int m_counterReadyBitIndex;

		int m_lineNumber;						// number of this line
		CMultiTracerDriverBase* m_parentPtr;	// parent object
	};

	/**	Called if application sent "keep alive" signal.
	 *		The "keep alive" signal indicate, that application works correctly.
	 *		If this signal doesn't come regulary, application is set to be in critical mode.
	 */
	void OnKeepAlive();

	void ReadHardwareValues(__int64 microsecsTimer, IDriver::NativeTimer internalTimer);
	void WriteHardwareValues();

	void CalcInterruptsMask();
	void ResetQueueLine(int lineIndex);

	// abstract methods
	virtual I_DWORD ReadPort() = 0;
	virtual void WritePort(I_DWORD value) = 0;
	virtual I_WORD ReadCounter(int counterIndex) = 0;
	virtual void WriteCounter(int counterIndex, I_WORD value) = 0;
	virtual void WriteInterruptsMask(I_DWORD value) = 0;

private:
	CMultiTracerMessages::MultiTracerParams m_params;

	SingleLine m_lines[MAX_LINES];

	I_DWORD m_interruptsMask;

	// temporaly stored hardware values
	I_DWORD m_inputBits;
	I_DWORD m_outputBits;
	I_DWORD m_lastOutputBits;

	// timer buffering
	__int64 m_microsecsTimer;
	IDriver::NativeTimer m_nativeTimer;
};


// inline methods

const CMultiTracerMessages::MultiTracerParams& CMultiTracerDriverBase::GetMultiTracerParams() const
{
	return m_params;
}


} // namespace ilolv


#endif // !ilolv_CMultiTracerDriverBase_included


