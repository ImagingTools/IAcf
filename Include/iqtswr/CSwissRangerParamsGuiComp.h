#ifndef iqtswr_CSwissRangerParamsGuiComp_included
#define iqtswr_CSwissRangerParamsGuiComp_included


#include "iswr/ISwissRangerParams.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtswr/Generated/ui_CSwissRangerParamsGuiComp.h"


namespace iqtswr
{


class CSwissRangerParamsGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CSwissRangerParamsGuiComp,
			iswr::ISwissRangerParams>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CSwissRangerParamsGuiComp,
				iswr::ISwissRangerParams> BaseClass;

	I_BEGIN_COMPONENT(CSwissRangerParamsGuiComp)
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();

	// reimplemented (iqtgui::CComponentBase)
	virtual void OnGuiCreated();

protected slots:
	void OnModelChanged();
};


} // namespace iqtswr


#endif // !iqtswr_CSwissRangerParamsGuiComp_included


