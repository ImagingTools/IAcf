#ifndef iqtcntl_CSimulatedMultiTracerGuiComp_included
#define iqtcntl_CSimulatedMultiTracerGuiComp_included


// Qt includes
#include <QTimer>

// ACF includes
#include "inat/CTimer.h"
#include "iqt/CCriticalSection.h"
#include "iqtgui/TDesignerGuiCompBase.h"

#include "ilolv/ICommandCaller.h"
#include "ilolv/CGeneralInfoDriverBase.h"
#include "ilolv/CMultiTracerDriverBase.h"
#include "ilolv/CSignalBitsDriverBase.h"

#include "iqtcntl/iqtcntl.h"

#include "iqtcntl/Generated/ui_CSimulatedMultiTracerGuiComp.h"


namespace iqtcntl
{


class CSimulatedMultiTracerGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CSimulatedMultiTracerGuiComp>,
			virtual public ilolv::ICommandCaller,
			protected ilolv::CGeneralInfoDriverBase,
			protected ilolv::CMultiTracerDriverBase,
			protected ilolv::CSignalBitsDriverBase
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CSimulatedMultiTracerGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CSimulatedMultiTracerGuiComp);
		I_REGISTER_INTERFACE(ilolv::ICommandCaller);
	I_END_COMPONENT;

	CSimulatedMultiTracerGuiComp();

	// reimplemented (ilolv::ICommandCaller)
	virtual bool CallCommand(
				int commandCode,
				const void* commandBuffer,
				int commandBufferSize,
				void* responseBuffer,
				int responseBufferSize,
				int& responseSize);

protected:
	// reimplemented (ilolv::CMultiTracerDriverBase)
	virtual I_WORD ReadCounter(int counterIndex);
	virtual void WriteCounter(int counterIndex, I_WORD value);
	virtual void WriteInterruptsMask(I_DWORD value);

	// reimplemented (ilolv::IDriver)
	virtual bool OnCommand(
				int commandCode,
				const void* commandBuffer,
				int commandBufferSize,
				void* responseBuffer,
				int responseBufferSize,
				int& responseSize);
	virtual void OnHardwareInterrupt(I_DWORD interruptFlags);
	virtual I_SQWORD GetCurrentTimer() const;
	virtual NativeTimer GetCurrentNativeTimer() const;

	// reimplemented (ilolv::IDigitalIo)
	virtual I_DWORD GetInputBits() const;
	virtual void SetOutputBits(I_DWORD value, I_DWORD mask);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

signals:
	void OutputChanged();

protected Q_SLOTS:
	void OnPeriodicTimer();
	void OnRunTimer();
	void OnInputChanged();
	void OnOutputChanged();
	void on_EncoderDial_valueChanged(int value);
	void on_RunButton_toggled(bool checked);

private:
	enum
	{
		OUTPUT_BITS_COUNT = 32
	};

	I_DWORD m_inputBits;
	I_DWORD m_outputBits;
	bool m_isCounterReady;
	I_WORD m_counterValue;
	I_DWORD m_interruptMask;
	int m_outputCounters[OUTPUT_BITS_COUNT];
	int m_lastEncoderValue;

	mutable inat::CTimer m_timer;
	I_SQWORD m_currentTimer;

	iqt::CCriticalSection m_globalSection;

	QTimer m_periodicTimer;
	QTimer m_runTimer;
};


} // namespace iqtcntl


#endif // !iqtcntl_CSimulatedMultiTracerGuiComp_included


