#ifndef iqtinsp_CGeneralSupplierGuiComp_included
#define iqtinsp_CGeneralSupplierGuiComp_included


#include "iproc/ISupplier.h"

#include "iqtinsp/TSupplierGuiCompBase.h"

#include "iqtinsp/Generated/ui_CGeneralSupplierGuiComp.h"


namespace iqtinsp
{


class CGeneralSupplierGuiComp: public TSupplierGuiCompBase<
			Ui::CGeneralSupplierGuiComp,
			iproc::ISupplier>
{
	Q_OBJECT

public:
	typedef TSupplierGuiCompBase<
				Ui::CGeneralSupplierGuiComp,
				iproc::ISupplier> BaseClass;

	I_BEGIN_COMPONENT(CGeneralSupplierGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

protected slots:
	void on_TestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

protected:
	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
};


} // namespace iqtinsp


#endif // !iqtinsp_CGeneralSupplierGuiComp_included


