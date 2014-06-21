#include "CChannelSelectionNode.h"


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "istd/CChangeNotifier.h"


namespace imebase
{


CChannelSelectionNode::CChannelSelectionNode()
:	m_selectedIndex(0)
{
}


void CChannelSelectionNode::InsertNode(const QString& name, int physicalAddress, CChannelSelectionNode* nodePtr)
{
	SelectionInfo* newNodeInfoPtr = new SelectionInfo;
	newNodeInfoPtr->name = name;
	newNodeInfoPtr->selectionPtr.SetPtr(nodePtr);
	newNodeInfoPtr->physicalIndex = physicalAddress;

	m_subselections.PushBack(newNodeInfoPtr);
}


int CChannelSelectionNode::GetActivePhysicalAddress() const
{
	if (m_selectedIndex >= GetOptionsCount()){
		return NULL;
	}

	const SelectionInfo* infoPtr = m_subselections.GetAt(m_selectedIndex);
	Q_ASSERT(infoPtr != NULL);

	return infoPtr->physicalIndex;
}


void CChannelSelectionNode::ResetNodes()
{
	m_subselections.Reset();
}


// reimplemented (iser::ISelectionParam)

const iprm::IOptionsList* CChannelSelectionNode::GetSelectionConstraints() const
{
	return this;
}


int CChannelSelectionNode::GetSelectedOptionIndex() const
{
	return m_selectedIndex;
}


bool CChannelSelectionNode::SetSelectedOptionIndex(int index)
{
	if ((index >= 0) && (index < GetOptionsCount())){
		m_selectedIndex = index;

		return true;
	}

	return false;
}


iprm::ISelectionParam* CChannelSelectionNode::GetSubselection(int /*index*/) const
{
	if (m_selectedIndex >= GetOptionsCount()){
		return NULL;
	}

	const SelectionInfo* infoPtr = m_subselections.GetAt(m_selectedIndex);
	Q_ASSERT(infoPtr != NULL);

	return infoPtr->selectionPtr.GetPtr();
}

	
// reimplemented (iprm::IOptionsList)

int CChannelSelectionNode::GetOptionsFlags() const
{
	return SCF_NONE;
}


int CChannelSelectionNode::GetOptionsCount() const
{
	return m_subselections.GetCount();
}


QString CChannelSelectionNode::GetOptionName(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < m_subselections.GetCount());

	const SelectionInfo* infoPtr = m_subselections.GetAt(index);
	Q_ASSERT(infoPtr != NULL);

	return infoPtr->name;
}


QString CChannelSelectionNode::GetOptionDescription(int /*index*/) const
{
	return QString();
}


QByteArray CChannelSelectionNode::GetOptionId(int /*index*/) const
{
	return QByteArray();
}


bool CChannelSelectionNode::IsOptionEnabled(int /*index*/) const
{
	return true;
}


// reimplemented (iser::ISerializable)

bool CChannelSelectionNode::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag addressTag("Address", "Address of complete Meilhaus channel");
	static iser::CArchiveTag selectionTag("Selection", "Selected index of single Meilhaus channel node");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

	bool retVal = true;

	int selectionDepth = 0;
	if (archive.IsStoring()){
		selectionDepth = CalcSelectionDepth();
	}

	retVal = retVal && archive.BeginMultiTag(addressTag, selectionTag, selectionDepth);
	retVal = retVal && SerializeNode(archive, selectionTag, selectionDepth);
	retVal = retVal && archive.EndTag(addressTag);

	return retVal;
}


// protected methods

int CChannelSelectionNode::CalcSelectionDepth() const
{
	const CChannelSelectionNode* selectionPtr = dynamic_cast<CChannelSelectionNode*>(GetSubselection(GetSelectedOptionIndex()));
	if (selectionPtr != NULL){
		return selectionPtr->CalcSelectionDepth() + 1;
	}

	return 1;
}


bool CChannelSelectionNode::SerializeNode(iser::IArchive& archive, const iser::CArchiveTag& nodeTag, int count)
{
	if (count <= 0){
		return true;
	}

	bool retVal = true;

	retVal = retVal && archive.BeginTag(nodeTag);
	retVal = retVal && archive.Process(m_selectedIndex);
	retVal = retVal && archive.EndTag(nodeTag);

	if (m_selectedIndex >= GetOptionsCount()){
		m_selectedIndex = qMax(0, GetOptionsCount() - 1);
	}

	if (!retVal){
		return false;
	}

	CChannelSelectionNode* selectionPtr = dynamic_cast<CChannelSelectionNode*>(GetSubselection(GetSelectedOptionIndex()));
	if (selectionPtr != NULL){
		return selectionPtr->SerializeNode(archive, nodeTag, count - 1);
	}
	else{
		while (--count > 0){
			int dummy = 0;
			retVal = retVal && archive.BeginTag(nodeTag);
			retVal = retVal && archive.Process(dummy);
			retVal = retVal && archive.EndTag(nodeTag);
		}
	}

	return retVal;
}


} // namespace imebase


