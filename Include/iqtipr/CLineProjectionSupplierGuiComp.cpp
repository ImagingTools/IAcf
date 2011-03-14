#include "iqtipr/CLineProjectionSupplierGuiComp.h"


// Qt includes
#include <QMessageBox>


// ACF includes
#include "imath/CVarVector.h"

#include "istd/TChangeNotifier.h"
#include "iser/CMemoryReadArchive.h"


// IACF includes
#include "iqtipr/CProjectionShape.h"


namespace iqtipr
{


// reimplemented (imod::IModelEditor)

void CLineProjectionSupplierGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated() && (GetObjectPtr() != NULL));
}


void CLineProjectionSupplierGuiComp::UpdateEditor(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	iipr::ILineProjectionSupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		const imeas::IDataSequence* projectionPtr = supplierPtr->GetLineProjection();
		if (projectionPtr != NULL){
			istd::CChangeNotifier changePtr(&m_projectionData);

			m_projectionData.CopyFrom(*projectionPtr);
		}
		else{
			m_projectionData.ResetSequence();
		}
	}
}


// protected slots

void CLineProjectionSupplierGuiComp::on_TestButton_clicked()
{
	DoTest();
}


void CLineProjectionSupplierGuiComp::on_LoadParamsButton_clicked()
{
	LoadParams();
}


void CLineProjectionSupplierGuiComp::on_SaveParamsButton_clicked()
{
	SaveParams();
}


// protected methods

// reimplemented (iqtinsp::TSupplierGuiCompBase)

QWidget* CLineProjectionSupplierGuiComp::GetParamsWidget() const
{
	I_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// reimplemented (iqt2d::TSceneExtenderCompBase)

void CLineProjectionSupplierGuiComp::CreateShapes(int /*sceneId*/, bool /*inactiveOnly*/, Shapes& result)
{
 	iqtipr::CProjectionShape* shapePtr = new iqtipr::CProjectionShape;
	if (shapePtr != NULL){
		result.PushBack(shapePtr);
	
		m_projectionData.AttachObserver(shapePtr);
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CLineProjectionSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());
	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());

	if (m_projectionObserverCompPtr.IsValid()){
		m_projectionData.AttachObserver(m_projectionObserverCompPtr.GetPtr());
	}
}


// reimplemented (iqtgui::IGuiObject)

void CLineProjectionSupplierGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_projectionObserverGuiCompPtr.IsValid()){
		m_projectionObserverGuiCompPtr->CreateGui(ProjectionFrame);
	}
}


void CLineProjectionSupplierGuiComp::OnGuiDestroyed()
{
	if (m_projectionObserverGuiCompPtr.IsValid()){
		m_projectionObserverGuiCompPtr->DestroyGui();
	}
	
	BaseClass::OnGuiDestroyed();
}


} // namespace iqtipr


