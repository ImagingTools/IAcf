#include "iqtinsp/CInspectionTaskGuiComp.h"


// Qt includes
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QToolBox>
#include <QTabWidget>
#include <QGroupBox>


// ACF includes
#include "imod/IModel.h"
#include "imod/IObserver.h"


namespace iqtinsp
{


CInspectionTaskGuiComp::CInspectionTaskGuiComp()
:	m_currentGuiIndex(-1)
{
}


// reimplemented (imod::IModelEditor)

void CInspectionTaskGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated() && (GetObjectPtr() != NULL));

	int editorsCount = m_editorsCompPtr.GetCount();
	for (int i = 0; i < editorsCount; ++i){
		imod::IModelEditor* editorPtr = m_editorsCompPtr[i];
		if (editorPtr != NULL){
			editorPtr->UpdateModel();
		}
	}
}


void CInspectionTaskGuiComp::UpdateEditor(int updateFlags)
{
	I_ASSERT(IsGuiCreated());

	int editorsCount = m_editorsCompPtr.GetCount();
	for (int i = 0; i < editorsCount; ++i){
		imod::IModelEditor* editorPtr = m_editorsCompPtr[i];
		if (editorPtr != NULL){
			editorPtr->UpdateEditor(updateFlags);
		}
	}
}


// reimplemented (imod::IObserver)

bool CInspectionTaskGuiComp::OnAttached(imod::IModel* modelPtr)
{
	if (!BaseClass::OnAttached(modelPtr)){
		return false;
	}

	iinsp::IInspectionTask* inspectionTaskPtr = GetObjectPtr();
	I_ASSERT(inspectionTaskPtr != NULL);

	int subtasksCount = istd::Min(m_observersCompPtr.GetCount(), inspectionTaskPtr->GetSubtasksCount());
	for (int i = 0; i < subtasksCount; ++i){
		imod::IModel* parameterModelPtr = dynamic_cast<imod::IModel*>(inspectionTaskPtr->GetSubtask(i));
		imod::IObserver* observerPtr = m_observersCompPtr[i];

		if ((parameterModelPtr != NULL) && (observerPtr != NULL)){
			parameterModelPtr->AttachObserver(observerPtr);
		}
	}

	return true;
}


bool CInspectionTaskGuiComp::OnDetached(imod::IModel* modelPtr)
{
	iinsp::IInspectionTask* inspectionTaskPtr = GetObjectPtr();
	I_ASSERT(inspectionTaskPtr != NULL);

	int subtasksCount = istd::Min(m_observersCompPtr.GetCount(), inspectionTaskPtr->GetSubtasksCount());
	for (int i = 0; i < subtasksCount; ++i){
		imod::IModel* parameterModelPtr = dynamic_cast<imod::IModel*>(inspectionTaskPtr->GetSubtask(i));
		imod::IObserver* observerPtr = m_observersCompPtr[i];

		if ((parameterModelPtr != NULL) && (observerPtr != NULL) && parameterModelPtr->IsAttached(observerPtr)){
			parameterModelPtr->DetachObserver(observerPtr);
		}
	}

	return BaseClass::OnDetached(modelPtr);
}


// reimplemented (iqtgui::CGuiComponentBase)

void CInspectionTaskGuiComp::OnGuiCreated()
{
	QLayout* layoutPtr = ParamsFrame->layout();
	if (layoutPtr == NULL){
		if (*m_useHorizontalLayoutAttrPtr){
			layoutPtr = new QHBoxLayout(ParamsFrame);
		}
		else{
			layoutPtr = new QVBoxLayout(ParamsFrame);
		}
	}

	if (!m_paramsLoaderCompPtr.IsValid()){
		LoaderFrame->hide();
	}

	layoutPtr->setMargin(0);

	ParamsFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

	if (*m_designTypeAttrPtr == 1){
		QToolBox* toolBoxPtr = new QToolBox(ParamsFrame);
		int subtasksCount = m_guisCompPtr.GetCount();
		for (int i = 0; i < subtasksCount; ++i){
			iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];

			if (guiPtr != NULL){
				QWidget* panelPtr = new QWidget(ParamsFrame);
				QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
				panelLayoutPtr->setContentsMargins(6, 0, 6, 0);
				QString name;
				if (i < m_namesAttrPtr.GetCount()){
					name = iqt::GetQString(m_namesAttrPtr[i]);
				}

				guiPtr->CreateGui(panelPtr);

				toolBoxPtr->addItem(panelPtr, name);

				QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

				panelLayoutPtr->addItem(spacerPtr);
			}
		}

		QObject::connect(toolBoxPtr, SIGNAL(currentChanged(int)), this, SLOT(OnEditorChanged(int)));

		layoutPtr->addWidget(toolBoxPtr);
	}
	else{
		QTabWidget* tabWidgetPtr = new QTabWidget(ParamsFrame);
		tabWidgetPtr->setTabPosition(QTabWidget::TabPosition(*m_tabOrientationAttrPtr));

		int subtasksCount = m_guisCompPtr.GetCount();
		for (int i = 0; i < subtasksCount; ++i){
			iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];

			if (guiPtr != NULL){
				QWidget* panelPtr = new QWidget(tabWidgetPtr);
				QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
				QString name;
				if (i < m_namesAttrPtr.GetCount()){
					name = iqt::GetQString(m_namesAttrPtr[i]);
				}

				guiPtr->CreateGui(panelPtr);

				tabWidgetPtr->addTab(panelPtr, name);

				QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

				panelLayoutPtr->addItem(spacerPtr);
			}
		}

		QObject::connect(tabWidgetPtr, SIGNAL(currentChanged(int)), this, SLOT(OnEditorChanged(int)));

		layoutPtr->addWidget(tabWidgetPtr);
	}

	std::map<iqtgui::IGuiObject*, int> guiToStackIndexMap;

	int previewGuisCount = m_previewGuisCompPtr.GetCount();
	for (int previewIndex = 0; previewIndex < previewGuisCount; ++previewIndex){
		iqtgui::IGuiObject* guiObjectPtr = m_previewGuisCompPtr[previewIndex];
		if (guiObjectPtr == NULL){
			continue;
		}

		int stackIndex;
		std::map<iqtgui::IGuiObject*, int>::const_iterator stackRepeatIter = guiToStackIndexMap.find(guiObjectPtr);
		if (stackRepeatIter == guiToStackIndexMap.end()){
			istd::TDelPtr<QWidget> newPreviewPagePtr(new QWidget());
			if (!newPreviewPagePtr.IsValid()){
				continue;
			}

			new QVBoxLayout(newPreviewPagePtr.GetPtr());
			newPreviewPagePtr->setContentsMargins(0, 0, 0, 0);

			if (!guiObjectPtr->CreateGui(newPreviewPagePtr.GetPtr())){
				continue;
			}

			stackIndex = PreviewStack->addWidget(newPreviewPagePtr.PopPtr());
			m_stackIndexToTabMap[stackIndex] = previewIndex;

			guiToStackIndexMap[guiObjectPtr] = stackIndex;
		}
		else{
			stackIndex = stackRepeatIter->second;
		}

		m_tabToStackIndexMap[previewIndex] = stackIndex;
	}

	OnEditorChanged(0);

	BaseClass::OnGuiCreated();
}


void CInspectionTaskGuiComp::OnGuiDestroyed()
{
	int subtasksCount = m_guisCompPtr.GetCount();
	for (int i = 0; i < subtasksCount; ++i){
		iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];

		if ((guiPtr != NULL) && guiPtr->IsGuiCreated()){
			guiPtr->DestroyGui();
		}
	}

	for (		GuiMap::const_iterator iter = m_stackIndexToTabMap.begin();
				iter != m_stackIndexToTabMap.begin();
				++iter){
		int guiIndex = iter->second;
		I_ASSERT(guiIndex >= 0);
		I_ASSERT(guiIndex < m_previewGuisCompPtr.GetCount());

		iqtgui::IGuiObject* guiPtr = m_previewGuisCompPtr[guiIndex];
		I_ASSERT(guiPtr != NULL);

		guiPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


// protected slots

void CInspectionTaskGuiComp::OnEditorChanged(int index)
{
	if (index != m_currentGuiIndex){
		int extendersCount = m_extendersCompPtr.GetCount();
		int previewProvidersCount = m_previewSceneProvidersCompPtr.GetCount();

		if ((m_currentGuiIndex >= 0) && (m_currentGuiIndex < extendersCount) && (m_currentGuiIndex < previewProvidersCount)){
			iqt2d::ISceneExtender* extenderPtr = m_extendersCompPtr[m_currentGuiIndex];
			iqt2d::ISceneProvider* previewProviderPtr = m_previewSceneProvidersCompPtr[m_currentGuiIndex];
			if ((extenderPtr != NULL) && (previewProviderPtr != NULL)){
				extenderPtr->RemoveItemsFromScene(previewProviderPtr);
			}
		}

		if ((index >= 0) && (index < extendersCount) && (index < previewProvidersCount)){
			iqt2d::ISceneExtender* extenderPtr = m_extendersCompPtr[index];
			iqt2d::ISceneProvider* previewProviderPtr = m_previewSceneProvidersCompPtr[index];
			if ((extenderPtr != NULL) && (previewProviderPtr != NULL)){
				extenderPtr->AddItemsToScene(previewProviderPtr, iqt2d::ISceneExtender::SF_DIRECT);
			}
		}

		m_currentGuiIndex = index;

		int stackIndex = m_tabToStackIndexMap[index];
		PreviewStack->setCurrentIndex(stackIndex);
	}
}


void CInspectionTaskGuiComp::on_TestAllButton_clicked()
{
	iinsp::IInspectionTask* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int subtasksCount = objectPtr->GetSubtasksCount();

		for (int invalidateIndex = 0; invalidateIndex < subtasksCount; ++invalidateIndex){
			iproc::ISupplier* subtaskPtr = objectPtr->GetSubtask(invalidateIndex);
			if (subtaskPtr != NULL){
				subtaskPtr->InvalidateSupplier();
			}
		}

		for (int finishIndex = 0; finishIndex < subtasksCount; ++finishIndex){
			iproc::ISupplier* subtaskPtr = objectPtr->GetSubtask(finishIndex);
			if (subtaskPtr != NULL){
				subtaskPtr->EnsureWorkFinished();
			}
		}
	}
}


void CInspectionTaskGuiComp::on_LoadParamsButton_clicked()
{
	iinsp::IInspectionTask* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		m_paramsLoaderCompPtr->LoadFromFile(*objectPtr);
	}
}


void CInspectionTaskGuiComp::on_SaveParamsButton_clicked()
{
	iinsp::IInspectionTask* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		m_paramsLoaderCompPtr->SaveToFile(*objectPtr);
	}
}


} // namespace iqtinsp


