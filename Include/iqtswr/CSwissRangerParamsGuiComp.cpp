#include "iqtswr/CSwissRangerParamsGuiComp.h"


#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqtswr
{


// reimplemented (imod::IModelEditor)

void CSwissRangerParamsGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	iswr::ISwissRangerParams* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	istd::TChangeNotifier<iswr::ISwissRangerParams> changePtr(objectPtr);

	objectPtr->SetAmplitudeThreshold(AmplitudeThresholdSpin->value() / 100.0);
	objectPtr->SetModulationFrequencyMode(ModulationFrequencyModeCombo->currentIndex());
	objectPtr->SetMedianFilterEnabled(MedianFilterCheck->isChecked());
	objectPtr->SetAdaptiveFilterEnabled(AdaptiveFilterCheck->isChecked());
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CSwissRangerParamsGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	iswr::ISwissRangerParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		iqt::CSignalBlocker block(GetWidget(), true);
		AmplitudeThresholdSpin->setValue(objectPtr->GetAmplitudeThreshold() * 100);
		ModulationFrequencyModeCombo->setCurrentIndex(objectPtr->GetModulationFrequencyMode());
		MedianFilterCheck->setChecked(objectPtr->IsMedianFilterEnabled());
		AdaptiveFilterCheck->setChecked(objectPtr->IsAdaptiveFilterEnabled());
	}
}



// reimplemented (iqtgui::CComponentBase)

void CSwissRangerParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	connect(ModulationFrequencyModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(OnModelChanged()));
	connect(AmplitudeThresholdSpin, SIGNAL(valueChanged(double)), this, SLOT(OnModelChanged()));
	connect(MedianFilterCheck, SIGNAL(stateChanged(int)), this, SLOT(OnModelChanged()));
}


// protected slots

void CSwissRangerParamsGuiComp::OnModelChanged()
{
	if (!IsUpdateBlocked()){
		UpdateBlocker blockUpdate(this);

		UpdateModel();
	}
}


} // namespace iqtswr


