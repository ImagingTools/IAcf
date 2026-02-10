// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-IAcf-Commercial
#include <iqsci/CTextEditorGuiComp.h>


namespace iqsci
{


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

// reimplemented (iqtgui::TGuiObserverWrap)

void CTextEditorGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	idoc::ITextDocument* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	CTextEditor* textEditPtr = GetQtWidget();
	Q_ASSERT(textEditPtr != NULL);

	objectPtr->SetText(textEditPtr->GetText());
}


void CTextEditorGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	CTextEditor* textEditPtr = GetQtWidget();
	Q_ASSERT(textEditPtr != NULL);

	idoc::ITextDocument* objectPtr = GetObservedObject();
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
	DoUpdateModel();
}


} // namespace iqsci


