#include "iqsci/CTextEditorGuiComp.h"


namespace iqsci
{


// reimplemented (imod::IModelEditor)

void CTextEditorGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	ibase::ITextDocument* objectPtr = GetObjectPtr();
	Q_ASSERT(objectPtr != NULL);

	CTextEditor* textEditPtr = GetQtWidget();
	Q_ASSERT(textEditPtr != NULL);

	objectPtr->SetText(textEditPtr->GetText());
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CTextEditorGuiComp::UpdateGui(int /*updateFlags*/)
{
	CTextEditor* textEditPtr = GetQtWidget();
	Q_ASSERT(textEditPtr != NULL);

	ibase::ITextDocument* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		textEditPtr->SetText(objectPtr->GetText());
	}
}


void CTextEditorGuiComp::OnGuiModelDetached()
{
	CTextEditor* textEditPtr = GetQtWidget();
	Q_ASSERT(textEditPtr != NULL);

	textEditPtr->SetText(QString());

	BaseClass::OnGuiModelDetached();
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CTextEditorGuiComp::GetCommands() const
{
	if (IsGuiCreated()){
		CTextEditor* textEditPtr = GetQtWidget();
		Q_ASSERT(textEditPtr != NULL);
		
		return textEditPtr->GetCommands();
	}

	return NULL;
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CTextEditorGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	CTextEditor* textEditorPtr = GetQtWidget();
	Q_ASSERT(textEditorPtr != NULL);
	if (textEditorPtr != NULL){
		connect(textEditorPtr, SIGNAL(DataChanged()), this, SLOT(OnTextChanged()));

		if (m_useFoldingAttrPtr.IsValid()){
			textEditorPtr->SetFoldingEnabled(*m_useFoldingAttrPtr);
		}

		if (m_languageAttrPtr.IsValid()){
			textEditorPtr->SetLanguage(*m_languageAttrPtr);
		}

		if (m_readOnlyAttrPtr.IsValid() && *m_readOnlyAttrPtr){
			textEditorPtr->SetReadOnly();
		}
	}
}


void CTextEditorGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	if (IsGuiCreated()){
		CTextEditor* textEditPtr = GetQtWidget();
		Q_ASSERT(textEditPtr != NULL);
	
		textEditPtr->OnRetranslate();
	}
}


// protected slots

void CTextEditorGuiComp::OnTextChanged()
{
	UpdateBlocker updateBlocker(this);

	UpdateModel();
}


} // namespace iqsci


