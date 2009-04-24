#include "iqtcntl/CAutomaticControllerGuiComp.h"


namespace iqtcntl
{


// reimplemented (imod::IModelEditor)

void CAutomaticControllerGuiComp::UpdateModel() const
{
	icntl::IAutomaticController* modelPtr = GetObjectPtr();
	if (IsGuiCreated() && (modelPtr != NULL)){
		modelPtr->SetWorkMode(AutomaticButton->isChecked()? icntl::IAutomaticController::WM_AUTOMATIC: icntl::IAutomaticController::WM_MANUAL);
	}
}


void CAutomaticControllerGuiComp::UpdateEditor(int /*updateFlags*/)
{
	const icntl::IAutomaticController* modelPtr = GetObjectPtr();
	if (IsGuiCreated() && (modelPtr != NULL)){
		AutomaticButton->setChecked(modelPtr->GetWorkMode() == icntl::IAutomaticController::WM_AUTOMATIC);
	}
}


// protected slots

void CAutomaticControllerGuiComp::on_AutomaticButton_toggled(bool /*checked*/)
{
	UpdateModel();
}


} // namespace iqtcntl


