#ifndef iqtswr_CSwissRangerParamsGuiComp_included
#define iqtswr_CSwissRangerParamsGuiComp_included


#include "iswr/ISwissRangerParams.h"
#include "iswr/ISwissRangerConstrains.h"

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
		I_ASSIGN(m_swissRangerConstrainsCompPtr, "SwissRangerConstrains", "Constrains of the swiss ranger sensor", false, "SwissRangerConstrains")
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CComponentBase)
	virtual void OnGuiCreated();

protected Q_SLOTS:
	void OnModelChanged();

private:
	I_REF(iswr::ISwissRangerConstrains, m_swissRangerConstrainsCompPtr);
};


} // namespace iqtswr


#endif // !iqtswr_CSwissRangerParamsGuiComp_included


