#ifndef iqtcntl_CAutomaticControllerGuiComp_included
#define iqtcntl_CAutomaticControllerGuiComp_included


// ACF includes
#include "imod/IModel.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "icntl/IAutomaticController.h"

#include "iqtcntl/iqtcntl.h"

#include "iqtcntl/Generated/ui_CAutomaticControllerGuiComp.h"


namespace iqtcntl
{


class CAutomaticControllerGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<Ui::CAutomaticControllerGuiComp, icntl::IAutomaticController>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CAutomaticControllerGuiComp, icntl::IAutomaticController> BaseClass;

	I_BEGIN_COMPONENT(CAutomaticControllerGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

protected Q_SLOTS:
	void on_AutomaticButton_toggled(bool checked);
};


} // namespace iqtcntl


#endif // !iqtcntl_CAutomaticControllerGuiComp_included


