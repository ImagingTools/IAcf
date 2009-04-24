#ifndef iqtipr_CSearchParamsGuiComp_h
#define iqtipr_CSearchParamsGuiComp_h


#include "imod/TModelWrap.h"

#include "iipr/CSearchParams.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqt/CBitmap.h"

#include "iqtipr/Generated/ui_CSearchParamsGuiComp.h"


namespace iqtipr
{


class CSearchParamsGuiComp: public iqtgui::TDesignerGuiObserverCompBase<Ui::CSearchParamsGuiComp, iipr::ISearchParams>
{
    Q_OBJECT
public:
	typedef  iqtgui::TDesignerGuiObserverCompBase<Ui::CSearchParamsGuiComp, iipr::ISearchParams> BaseClass;

	I_BEGIN_COMPONENT(CSearchParamsGuiComp)
		I_ASSIGN(m_modelViewCompPtr, "ModelView", "Model view", false, "ModelView");
		I_ASSIGN(m_modelObserverCompPtr, "ModelView", "Model view", false, "ModelView");
	I_END_COMPONENT

		// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor(int updateFlags = 0);
	virtual void UpdateModel() const;

protected slots:
	void OnParameterChanged();

protected:
	// reimplemented (iqt::CGuiObjectBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	imod::TModelWrap<iqt::CBitmap> m_modelBitmap;

	I_REF(iqtgui::IGuiObject, m_modelViewCompPtr);
	I_REF(imod::IObserver, m_modelObserverCompPtr);
};


} // namespace iqtipr


#endif // !iqtipr_CSearchParamsGuiComp_h

