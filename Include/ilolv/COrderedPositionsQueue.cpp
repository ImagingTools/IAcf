#include "ilolv/COrderedPositionsQueue.h"


namespace ilolv
{


bool COrderedPositionsQueue::Insert(I_DWORD position)
{
	int nextIndex = GetNextIndex(m_nextBackIndex);

	if (nextIndex != m_frontIndex){
		int insertionIndex = m_nextBackIndex;
		while (insertionIndex != m_frontIndex){	// shift all elements to assume correct elements order
			int prevInsertionIndex = GetPrevIndex(insertionIndex);

			if (I_SDWORD(m_elements[prevInsertionIndex] - position) <= 0){
				break;
			}

			m_elements[insertionIndex] = m_elements[prevInsertionIndex];

			insertionIndex = prevInsertionIndex;
		}

		m_elements[insertionIndex] = position;

		m_nextBackIndex = nextIndex;

		return true;
	}

	return false;
}


bool COrderedPositionsQueue::PopFrontTill(I_DWORD position)
{
	if (!IsEmpty() && (I_SDWORD(GetFront() - position) <= 0)){
		PopFront();

		while (!IsEmpty() && (I_SDWORD(GetFront() - position) <= 0)){
			PopFront();
		}

		return true;
	}

	return false;
}


} // namespace ilolv


