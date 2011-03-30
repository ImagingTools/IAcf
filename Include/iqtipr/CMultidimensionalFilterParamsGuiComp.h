#ifndef iqt_CMultidimensionalFilterParamsGuiComp_included
#define iqt_CMultidimensionalFilterParamsGuiComp_included


#include "iipr/IMultidimensionalFilterParams.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtipr/Generated/ui_CMultidimensionalFilterParamsGuiComp.h"


namespace iqtipr
{


class CMultidimensionalFilterParamsGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CMultidimensionalFilterParamsGuiComp,
			iipr::IMultidimensionalFilterParams>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CMultidimensionalFilterParamsGuiComp,
				iipr::IMultidimensionalFilterParams> BaseClass;

	I_BEGIN_COMPONENT(CMultidimensionalFilterParamsGuiComp)
	I_END_COMPONENT

	CMultidimensionalFilterParamsGuiComp();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

protected:
	void UpdateLabel();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();

protected slots:
	void on_FilterWidthSlider_valueChanged(int value);
	void on_FilterHeightSlider_valueChanged(int value);

private:
	double m_widthScaleFactor;
	double m_heightScaleFactor;
};


} // namespace iqtipr


#endif // !iqt_CMultidimensionalFilterParamsGuiComp_included


