#ifndef iqsci_CTextEditorGuiComp_included
#define iqsci_CTextEditorGuiComp_included


// ACF includes
#include <imod/TSingleModelObserverBase.h>

#include <idoc/ITextDocument.h>

#include <iqtgui/TGuiComponentBase.h>
#include <iqtgui/TGuiObserverWrap.h>

#include <iqsci/CTextEditor.h>


namespace iqsci
{


class CTextEditorGuiComp:
			public iqtgui::TGuiObserverWrap<
					iqtgui::TGuiComponentBase<CTextEditor>,
					imod::TSingleModelObserverBase<idoc::ITextDocument> >,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqtgui::TGuiObserverWrap<
				iqtgui::TGuiComponentBase<CTextEditor>,
				imod::TSingleModelObserverBase<idoc::ITextDocument> > BaseClass;

	I_BEGIN_COMPONENT(CTextEditorGuiComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_ASSIGN(m_languageAttrPtr, "Language", "Use syntax highlighting for defined language", false, "Language");
		I_ASSIGN(m_useFoldingAttrPtr, "UseFolding", "Use folding in the text document", false, true);
		I_ASSIGN(m_readOnlyAttrPtr, "ReadOnly", "Document view is read only", false, false);
	I_END_COMPONENT;

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);
	virtual void OnGuiModelDetached();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiRetranslate();

protected Q_SLOTS:
	virtual void OnTextChanged();

private:
	I_ATTR(bool, m_useFoldingAttrPtr);
	I_ATTR(QString, m_languageAttrPtr);
	I_ATTR(bool, m_readOnlyAttrPtr);
};


} // namespace iqsci


#endif // !iqsci_CTextEditorGuiComp_included


