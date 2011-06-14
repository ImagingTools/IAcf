#include "iqtcntl/CAutomaticControllerGuiComp.h"


namespace iqtcntl
{


// reimplemented (imod::IModelEditor)

void CAutomaticControllerGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	icntl::IAutomaticController* modelPtr = GetObjectPtr();
	I_ASSERT(modelPtr != NULL);

	modelPtr->SetWorkMode(AutomaticButton->isChecked()? icntl::IAutomaticController::WM_AUTOMATIC: icntl::IAutomaticController::WM_MANUAL);
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CAutomaticControllerGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	const icntl::IAutomaticController* modelPtr = GetObjectPtr();
	if (IsGuiCreated() && (modelPtr != NULL)){
		AutomaticButton->setChecked(modelPtr->GetWorkMode() == icntl::IAutomaticController::WM_AUTOMATIC);
	}
}


// protected slots

void CAutomaticControllerGuiComp::on_AutomaticButton_toggled(bool /*checked*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker blockUpdate(this);

		UpdateModel();
	}
}


} // namespace iqtcntl


