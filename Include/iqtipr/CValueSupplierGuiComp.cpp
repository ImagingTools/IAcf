#include "iqtipr/CValueSupplierGuiComp.h"


// Qt includes
#include <QMessageBox>

// ACF includes
#include "imath/CVarVector.h"
#include "iqt2d/CPosition2dShape.h"
#include "iqt2d/CCircleShape.h"


namespace iqtipr
{


CValueSupplierGuiComp::CValueSupplierGuiComp()
:	m_paramsObserver(this)
{
}


// reimplemented (imod::IModelEditor)

void CValueSupplierGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated() && (GetObjectPtr() != NULL));
}


void CValueSupplierGuiComp::UpdateEditor(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	iproc::IValueSupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		imath::CVarVector position;

		int workStatus = supplierPtr->GetWorkStatus();
		if (workStatus == iproc::ISupplier::WS_OK){
			position = supplierPtr->GetValue(-1, iproc::IValueSupplier::VTI_POSITION);
		}

		if (workStatus >= iproc::ISupplier::WS_OK){
			bool isResultVisible = false;
			if (position.GetElementsCount() >= 2){
				m_foundModel.SetPosition(i2d::CVector2d(position[0], position[1]));

				isResultVisible = true;

				if (IsGuiCreated()){
					PositionLabel->setText(tr("(%1, %2)").arg(position[0]).arg(position[1]));
				}
			}
			else{
				m_foundModel.SetPosition(i2d::CVector2d(0, 0));

				if (IsGuiCreated()){
					PositionLabel->setText("No position");
				}
			}

			imath::CVarVector radius = supplierPtr->GetValue(-1, iproc::IValueSupplier::VTI_RADIUS);
			if (radius.GetElementsCount() >= 1){
				m_foundModel.SetRadius(radius[0]);
			}
			else{
				m_foundModel.SetRadius(0);
			}

			int shapesCount = m_foundModel.GetObserverCount();
			for (int i = 0; i < shapesCount; ++i){
				QGraphicsItem* shapePtr = dynamic_cast<QGraphicsItem*>(m_foundModel.GetObserverPtr(i));
				shapePtr->setVisible(isResultVisible);
			}

			imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(supplierPtr->GetModelParametersSet());
			if (paramsModelPtr != NULL){
				if (!paramsModelPtr->IsAttached(&m_paramsObserver)){
					m_paramsObserver.EnsureDetached();

					paramsModelPtr->AttachObserver(&m_paramsObserver);
				}
			}
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
	iqt2d::CCircleShape* circleShapePtr = new iqt2d::CCircleShape(false);
	if (circleShapePtr != NULL){
		circleShapePtr->setZValue(2);
		circleShapePtr->setVisible(false);

		m_foundModel.AttachObserver(circleShapePtr);

		result.PushBack(circleShapePtr);
	}

	iqt2d::CPosition2dShape* posShapePtr = new iqt2d::CPosition2dShape(false);
	if (posShapePtr != NULL){
		posShapePtr->setZValue(2);
		posShapePtr->setVisible(false);

		m_foundModel.AttachObserver(posShapePtr);

		result.PushBack(posShapePtr);
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


// public methods of embedded class ParamsObserver

CValueSupplierGuiComp::ParamsObserver::ParamsObserver(CValueSupplierGuiComp* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// reimplemented (imod::CSingleModelObserverBase)

void CValueSupplierGuiComp::ParamsObserver::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (		((updateFlags & istd::IChangeable::CF_MODEL) != 0) &&
				m_parent.IsGuiCreated() &&
				m_parent.AutoUpdateButton->isChecked()){
		m_parent.DoTest();
	}
}


} // namespace iqtipr


