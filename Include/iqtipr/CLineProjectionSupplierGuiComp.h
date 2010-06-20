#ifndef iqtipr_CLineProjectionSupplierGuiComp_included
#define iqtipr_CLineProjectionSupplierGuiComp_included


// ACF includes
#include "iser/IFileLoader.h"
#include "imod/IObserver.h"
#include "imod/TModelWrap.h"
#include "i2d/CPosition2d.h"
#include "iproc/IValueSupplier.h"
#include "iqtgui/IGuiObject.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtproc/TSupplierGuiCompBase.h"

// ACF-SOlutions includes
#include "imeas/CGeneralDataSequence.h"

#include "iipr/ILineProjectionSupplier.h"

#include "iqtipr/iqtipr.h"

#include "iqtipr/Generated/ui_CLineProjectionSupplierGuiComp.h"


namespace iqtipr
{


class CLineProjectionSupplierGuiComp: public iqtproc::TSupplierGuiCompBase<
			Ui::CLineProjectionSupplierGuiComp,
			iipr::ILineProjectionSupplier>
{
	Q_OBJECT

public:
	typedef iqtproc::TSupplierGuiCompBase<
				Ui::CLineProjectionSupplierGuiComp,
				iipr::ILineProjectionSupplier> BaseClass;

	I_BEGIN_COMPONENT(CLineProjectionSupplierGuiComp);
		I_ASSIGN(m_projectionObserverCompPtr, "ProjectionObserver", "Observer for the generated line projection", true, "ProjectionObserver");
		I_ASSIGN(m_projectionObserverGuiCompPtr, "ProjectionObserver", "Observer for the generated line projection", true, "ProjectionObserver");
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

protected slots:
	void on_TestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

protected:
	// reimplemented (iqtproc::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;

	// reimplemented (iqt2d::TSceneExtenderCompBase)
	virtual void CreateShapes(int sceneId, bool inactiveOnly, Shapes& result);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	imod::TModelWrap<imeas::CGeneralDataSequence> m_projectionData;

	I_REF(imod::IObserver, m_projectionObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_projectionObserverGuiCompPtr);
};


} // namespace iqtipr


#endif // !iqtipr_CLineProjectionSupplierGuiComp_included


