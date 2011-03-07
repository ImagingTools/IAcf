#ifndef iqt_CRectangularFilterParamsGuiComp_included
#define iqt_CRectangularFilterParamsGuiComp_included


#include "iipr/IMultidimensionalFilterParams.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtipr/Generated/ui_CRectangularFilterParamsGuiComp.h"


namespace iqtipr
{


class CRectangularFilterParamsGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CRectangularFilterParamsGuiComp,
			iipr::IMultidimensionalFilterParams>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CRectangularFilterParamsGuiComp,
				iipr::IMultidimensionalFilterParams> BaseClass;

	I_BEGIN_COMPONENT(CRectangularFilterParamsGuiComp)
	I_END_COMPONENT

	CRectangularFilterParamsGuiComp();

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


#endif // !iqt_CRectangularFilterParamsGuiComp_included


