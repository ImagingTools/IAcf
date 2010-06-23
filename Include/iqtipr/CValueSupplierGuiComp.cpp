#include "iqtipr/CValueSupplierGuiComp.h"


// Qt includes
#include <QMessageBox>

// ACF includes
#include "imath/TVarVector.h"
#include "iqt2d/CPosition2dShape.h"


namespace iqtipr
{


// reimplemented (imod::IModelEditor)

void CValueSupplierGuiComp::UpdateModel() const
{
}


void CValueSupplierGuiComp::UpdateEditor(int /*updateFlags*/)
{
	iproc::IValueSupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		bool isResultVisible = false;
		imath::CVarVector position = supplierPtr->GetValue();
		if (position.GetElementsCount() >= 2){
			m_foundPosition.SetPosition(i2d::CVector2d(position[0], position[1]));
			isResultVisible = true;

			if (IsGuiCreated()){
				PositionLabel->setText(tr("(%1, %2)").arg(position[0]).arg(position[1]));
			}
		}
		else{
			m_foundPosition.SetPosition(i2d::CVector2d(0, 0));

			if (IsGuiCreated()){
				PositionLabel->setText("No position");
			}
		}

		int shapesCount = m_foundPosition.GetObserverCount();
		for (int i = 0; i < shapesCount; ++i){
			QGraphicsItem* shapePtr = dynamic_cast<QGraphicsItem*>(m_foundPosition.GetObserverPtr(i));
			shapePtr->setVisible(isResultVisible);
		}
	}
}


// protected slots

void CValueSupplierGuiComp::on_TestButton_clicked()
{
	DoTest();
}


void CValueSupplierGuiComp::on_LoadParamsButton_clicked()
{
	LoadParams();
}


void CValueSupplierGuiComp::on_SaveParamsButton_clicked()
{
	SaveParams();
}


// protected methods

// reimplemented (iqtproc::TSupplierGuiCompBase)

QWidget* CValueSupplierGuiComp::GetParamsWidget() const
{
	I_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// reimplemented (iqt2d::TSceneExtenderCompBase)

void CValueSupplierGuiComp::CreateShapes(int /*sceneId*/, bool /*inactiveOnly*/, Shapes& result)
{
	iqt2d::CPosition2dShape* shapePtr = new iqt2d::CPosition2dShape(false);
	if (shapePtr != NULL){
		shapePtr->setZValue(2);
		shapePtr->setVisible(false);

		m_foundPosition.AttachObserver(shapePtr);

		result.PushBack(shapePtr);
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CValueSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());
	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());
}


// reimplemented (iqtgui::IGuiObject)

void CValueSupplierGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_intermediateResultsGuiCompPtr.IsValid()){
		m_intermediateResultsGuiCompPtr->CreateGui(IntResultsFrame);

		IntResultsFrame->setVisible(true);
	}
	else{
		IntResultsFrame->setVisible(false);
	}
}


void CValueSupplierGuiComp::OnGuiDestroyed()
{
	if (m_intermediateResultsGuiCompPtr.IsValid()){
		m_intermediateResultsGuiCompPtr->DestroyGui();
	}
	
	BaseClass::OnGuiDestroyed();
}

} // namespace iqtipr


