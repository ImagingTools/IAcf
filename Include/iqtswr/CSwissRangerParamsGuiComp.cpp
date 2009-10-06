#include "iqtswr/CSwissRangerParamsGuiComp.h"


#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqtswr
{


// reimplemented (imod::IModelEditor)

void CSwissRangerParamsGuiComp::UpdateModel() const
{
	iswr::ISwissRangerParams* objectPtr = GetObjectPtr();
	if (IsGuiCreated() && (objectPtr != NULL) && !IsUpdateBlocked()){
		istd::TChangeNotifier<iswr::ISwissRangerParams> changePtr(objectPtr);
		UpdateBlocker block(const_cast<CSwissRangerParamsGuiComp*>(this));

		objectPtr->SetDistanceClipRange(istd::CRange(
					DistanceClippingFromSpin->value() / 100.0,
					DistanceClippingToSpin->value() / 100.0));
		objectPtr->SetAmplitudeThreshold(AmplitudeThresholdSpin->value() / 100.0);
		objectPtr->SetModulationFrequencyMode(ModulationFrequencyModeCombo->currentIndex());
		objectPtr->SetMedianFilterEnabled(MedianFilterCheck->isChecked());
		objectPtr->SetAdaptiveFilterEnabled(AdaptiveFilterCheck->isChecked());
	}
}


void CSwissRangerParamsGuiComp::UpdateEditor(int /*updateFlags*/)
{
	iswr::ISwissRangerParams* objectPtr = GetObjectPtr();
	if (IsGuiCreated() && (objectPtr != NULL)){
		iqt::CSignalBlocker block(GetWidget(), true);
		AmplitudeThresholdSpin->setValue(objectPtr->GetAmplitudeThreshold() * 100);
		DistanceClippingFromSpin->setValue(objectPtr->GetDistanceClipRange().GetMinValue() * 100);
		DistanceClippingToSpin->setValue(objectPtr->GetDistanceClipRange().GetMaxValue() * 100);
		ModulationFrequencyModeCombo->setCurrentIndex(objectPtr->GetModulationFrequencyMode());
		MedianFilterCheck->setChecked(objectPtr->IsMedianFilterEnabled());
		AdaptiveFilterCheck->setChecked(objectPtr->IsAdaptiveFilterEnabled());
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CSwissRangerParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();
}


// reimplemented (iqtgui::CComponentBase)

void CSwissRangerParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	connect(ModulationFrequencyModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(OnModelChanged()));
	connect(AmplitudeThresholdSpin, SIGNAL(valueChanged(double)), this, SLOT(OnModelChanged()));
	connect(DistanceClippingFromSpin, SIGNAL(valueChanged(double)), this, SLOT(OnModelChanged()));
	connect(DistanceClippingToSpin, SIGNAL(valueChanged(double)), this, SLOT(OnModelChanged()));
	connect(MedianFilterCheck, SIGNAL(stateChanged(int)), this, SLOT(OnModelChanged()));
}


// protected slots

void CSwissRangerParamsGuiComp::OnModelChanged()
{
	UpdateModel();
}


} // namespace iqtswr


