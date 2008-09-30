#ifndef ilolv_COrderedPositionsQueue_included
#define ilolv_COrderedPositionsQueue_included


#include "ilolv/ilolv.h"


namespace ilolv
{


struct COrderedPositionsQueue
{
public:
	enum
	{
		SIZE_BITS = 6,
		MAX_SIZE = 1 << SIZE_BITS,
		INDEX_MASK = MAX_SIZE - 1
	};

	/**
		Check if this queue is empty.
	*/
	bool IsEmpty() const;

	/**
		Remove all elements from this queue.
	*/
	void Reset();

	/**
		Insert new position to the queue.
	*/
	bool Insert(I_DWORD position);

	/**
		Get front position.
	*/
	I_DWORD GetFront() const;

	/**
		Remove front position.
	*/
	void PopFront();

	/**
		Pop front elements till all rest elements are back of specified position.
		\return	true	if some element was removed.
	*/
	bool PopFrontTill(I_DWORD position);

protected:
	// static methods
	static int GetPrevIndex(int fifoIndex);
	static int GetNextIndex(int fifoIndex);

private:
	I_DWORD m_elements[MAX_SIZE];

	int m_nextBackIndex;
	int m_frontIndex;
};


// inline methods

inline bool COrderedPositionsQueue::IsEmpty() const
{
	return m_frontIndex == m_nextBackIndex;
}


inline void COrderedPositionsQueue::Reset()
{
	m_nextBackIndex = 0;
	m_frontIndex = 0;
}


inline I_DWORD COrderedPositionsQueue::GetFront() const
{
	I_ASSERT(!IsEmpty());

	return m_elements[m_frontIndex];
}


inline void COrderedPositionsQueue::PopFront()
{
	I_ASSERT(!IsEmpty());

	m_frontIndex = GetNextIndex(m_frontIndex);
}


// protected inline methods

// static methods

inline int COrderedPositionsQueue::GetPrevIndex(int fifoIndex)
{
	return (fifoIndex - 1) & INDEX_MASK;
}


inline int COrderedPositionsQueue::GetNextIndex(int fifoIndex)
{
	return (fifoIndex + 1) & INDEX_MASK;
}


} // namespace ilolv


#endif // !ilolv_COrderedPositionsQueue_included


