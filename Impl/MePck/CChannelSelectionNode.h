#ifndef imebase_CChannelSelectionNode_included
#define imebase_CChannelSelectionNode_included


#include "istd/TDelPtr.h"
#include "istd/TPointerVector.h"

#include "iser/CArchiveTag.h"

#include "iprm/ISelectionParam.h"
#include "iprm/ISelectionConstraints.h"

#include "imebase.h"


namespace imebase
{


class CChannelSelectionNode:
			virtual public iprm::ISelectionParam,
			virtual public iprm::ISelectionConstraints
{
public:
	CChannelSelectionNode();

	/**
		Insert new node at the end of selection list.
		This node will be automatically deleted during destruction.
	*/
	void InsertNode(const istd::CString& name, int physicalAddress, CChannelSelectionNode* nodePtr);
	/**
		Get physical address associated with active selection.
	*/
	int GetActivePhysicalAddress() const;

	/**
		Remove all option nodes.
	*/
	void ResetNodes();

	// reimplemented (iprm::ISelectionParam)
	virtual const iprm::ISelectionConstraints* GetConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual iprm::ISelectionParam* GetActiveSubselection() const;

	// reimplemented (iprm::ISelectionConstraints)
	virtual int GetOptionsCount() const;
	virtual const istd::CString& GetOptionName(int index) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	int CalcSelectionDepth() const;
	bool SerializeNode(iser::IArchive& archive, const iser::CArchiveTag& nodeTag, int count);

private:
	struct SelectionInfo
	{
		istd::CString name;
		istd::TDelPtr<ISelectionParam> selectionPtr;
		int physicalIndex;
	};

	istd::TPointerVector<SelectionInfo> m_subselections;
	int m_selectedIndex;
};

	
} // namespace imebase


#endif // !imebase_CChannelSelectionNode_included


