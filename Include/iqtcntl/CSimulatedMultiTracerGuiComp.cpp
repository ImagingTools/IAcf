#include "iqtcntl/CSimulatedMultiTracerGuiComp.h"


// Qt includes
#include <QTableWidgetItem>
#include <QCheckBox>

// ACF includes
#include "isys/CSectionBlocker.h"


namespace iqtcntl
{


CSimulatedMultiTracerGuiComp::CSimulatedMultiTracerGuiComp()
:	m_inputBits(0),
	m_outputBits(0),
	m_isCounterReady(false),
	m_counterValue(0),
	m_interruptMask(0),
	m_currentTimer(0),
	m_lastEncoderValue(0)
{
	for (int i = 0; i < OUTPUT_BITS_COUNT; ++i){
		m_outputCounters[i] = 0;
	}

	QObject::connect(this, SIGNAL(OutputChanged()), this, SLOT(OnOutputChanged()), Qt::QueuedConnection);
	QObject::connect(&m_periodicTimer, SIGNAL(timeout()), this, SLOT(OnPeriodicTimer()));
	QObject::connect(&m_runTimer, SIGNAL(timeout()), this, SLOT(OnRunTimer()));
}


// reimplemented (ilolv::ICommandCaller)

bool CSimulatedMultiTracerGuiComp::CallCommand(
			int commandCode,
			const void* commandBuffer,
			int commandBufferSize,
			void* responseBuffer,
			int responseBufferSize,
			int& responseSize)
{
	return OnCommand(commandCode, commandBuffer, commandBufferSize, responseBuffer, responseBufferSize, responseSize);
}


// reimplemented (icomp::IComponent)

void CSimulatedMultiTracerGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_periodicTimer.start(10);
}


void CSimulatedMultiTracerGuiComp::OnComponentDestroyed()
{
	m_periodicTimer.stop();
	m_runTimer.stop();

	BaseClass::OnComponentDestroyed();
}


// protected methods

// reimplemented (ilolv::CMultiTracerDriverBase)

I_WORD CSimulatedMultiTracerGuiComp::ReadCounter(int counterIndex)
{
	if (counterIndex == 0){
		return m_counterValue;
	}

	return 0;
}


void CSimulatedMultiTracerGuiComp::WriteCounter(int counterIndex, I_WORD value)
{
	if (counterIndex == 0){
		m_isCounterReady = false;

		m_counterValue = value;
	}
}


void CSimulatedMultiTracerGuiComp::WriteInterruptsMask(I_DWORD value)
{
	m_interruptMask = value;
}


// reimplemented (ilolv::IDriver)

bool CSimulatedMultiTracerGuiComp::OnCommand(
			int commandCode,
			const void* commandBuffer,
			int commandBufferSize,
			void* responseBuffer,
			int responseBufferSize,
			int& responseSize)
{
	isys::CSectionBlocker blocker(&m_globalSection);

	CopyFromHardware();

	return		ilolv::CGeneralInfoDriverBase::OnCommand(commandCode, commandBuffer, commandBufferSize, responseBuffer, responseBufferSize, responseSize) ||
				ilolv::CMultiTracerDriverBase::OnCommand(commandCode, commandBuffer, commandBufferSize, responseBuffer, responseBufferSize, responseSize) ||
				ilolv::CSignalBitsDriverBase::OnCommand(commandCode, commandBuffer, commandBufferSize, responseBuffer, responseBufferSize, responseSize);
}


void CSimulatedMultiTracerGuiComp::OnHardwareInterrupt(I_DWORD interruptFlags)
{
	isys::CSectionBlocker blocker(&m_globalSection);

	CopyFromHardware();

	ilolv::CGeneralInfoDriverBase::OnHardwareInterrupt(interruptFlags);
	ilolv::CMultiTracerDriverBase::OnHardwareInterrupt(
					m_isCounterReady?
					interruptFlags | ilolv::CIoCardTracerDriverBase::IF_ENCODER_INTERRUPT:
					interruptFlags);
	ilolv::CSignalBitsDriverBase::OnHardwareInterrupt(interruptFlags);
}


__int64 CSimulatedMultiTracerGuiComp::GetCurrentTimer() const
{
	static iwin::CTimer referenceTimer;

	m_timer.Start();

	return I_DWORD(1e6 * m_timer.GetTimeTo(referenceTimer));
}


ilolv::IDriver::NativeTimer CSimulatedMultiTracerGuiComp::GetCurrentNativeTimer() const
{
	m_timer.Start();

	return m_timer.GetNativeRepresentation();
}


// reimplemented (ilolv::IDigitalIo)

I_DWORD CSimulatedMultiTracerGuiComp::GetInputBits() const
{
	return m_inputBits;
}


void CSimulatedMultiTracerGuiComp::SetOutputBits(I_DWORD value, I_DWORD mask)
{
	I_ASSERT(((~mask) & value) == 0);

	I_DWORD newOutputBits = value | (~mask & m_outputBits);

	if (newOutputBits != m_outputBits){
		for (int i = 0; i < OUTPUT_BITS_COUNT; ++i){
			if ((((newOutputBits >> i) & 1) == 0) && (((m_outputBits >> i) & 1) != 0)){
				m_outputCounters[i]++;
			}
		}

		m_outputBits = newOutputBits;

		emit OutputChanged();
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CSimulatedMultiTracerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	int bitsCount = BitsTable->rowCount();
	for (int i = 0; i < bitsCount; ++i){
		QCheckBox* inputWidgetPtr = new QCheckBox("");
		QObject::connect(inputWidgetPtr, SIGNAL(toggled(bool)), this, SLOT(OnInputChanged()));
		BitsTable->setCellWidget(i, 0, inputWidgetPtr);

		QCheckBox* outputWidgetPtr = new QCheckBox("");
		outputWidgetPtr->setEnabled(false);
		BitsTable->setCellWidget(i, 1, outputWidgetPtr);

		QLabel* counterWidgetPtr = new QLabel("-");
		BitsTable->setCellWidget(i, 2, counterWidgetPtr);
	}
}


// protected slots

void CSimulatedMultiTracerGuiComp::OnPeriodicTimer()
{
	OnHardwareInterrupt(IF_PULSE_TIMER);
}


void CSimulatedMultiTracerGuiComp::OnRunTimer()
{
	EncoderDial->setValue((EncoderSB->value() + SpeedSB->value()) % EncoderDial->maximum());
}


void CSimulatedMultiTracerGuiComp::OnInputChanged()
{
	if (IsGuiCreated()){
		I_DWORD newInputBits = 0;

		int bitsCount = BitsTable->rowCount();
		for (int i = bitsCount - 1; i >= 0; --i){
			newInputBits <<= 1;

			const QCheckBox* widgetPtr = dynamic_cast<QCheckBox*>(BitsTable->cellWidget(i, 0));
			if ((widgetPtr != NULL) && widgetPtr->isChecked()){
				newInputBits |= 1;
			}
		}

		if ((newInputBits & m_interruptMask) != (m_inputBits & m_interruptMask)){
			m_inputBits = newInputBits;

			OnHardwareInterrupt(IF_DIGITAL_INPUT);
		}
		else{
			m_inputBits = newInputBits;
		}
	}
}


void CSimulatedMultiTracerGuiComp::OnOutputChanged()
{
	if (IsGuiCreated()){
		int bitsCount = BitsTable->rowCount();
		for (int i = 0; i < bitsCount; ++i){
			QCheckBox* outputItemPtr = dynamic_cast<QCheckBox*>(BitsTable->cellWidget(i, 1));
			if (outputItemPtr != NULL){
				bool state = (((m_outputBits >> i) & 1) != 0);

				outputItemPtr->setChecked(state);
			}

			if (i < OUTPUT_BITS_COUNT){
				QLabel* countItemPtr = dynamic_cast<QLabel*>(BitsTable->cellWidget(i, 2));
				if (countItemPtr != NULL){
					countItemPtr->setText(QString::number(m_outputCounters[i]));
				}
			}
		}
	}
}


void CSimulatedMultiTracerGuiComp::on_EncoderDial_valueChanged(int value)
{
	I_WORD diff = I_WORD(value - m_lastEncoderValue & 4095);
	if (diff < 2000){
		m_counterValue -= diff;
		if (m_counterValue > 0x7fff){
			m_isCounterReady = true;

			OnHardwareInterrupt(IF_ENCODER_INTERRUPT);
		}
	}

	m_lastEncoderValue = value;
}


void CSimulatedMultiTracerGuiComp::on_RunButton_toggled(bool checked)
{
	if (checked){
		m_runTimer.start(200);
	}
	else{
		m_runTimer.stop();
	}
}


} // namespace iqtcntl


