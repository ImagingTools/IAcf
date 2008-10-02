#ifndef ilolv_TOrderedPositionsQueue_included
#define ilolv_TOrderedPositionsQueue_included


#include "ilolv/ilolv.h"


namespace ilolv
{


template <class Object, int SizeBits = 6>
struct TOrderedPositionsQueue
{
public:
	enum
	{
		MAX_SIZE = 1 << SizeBits,
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
		\param	position	insertion position.
							Objects will be automatically sorted.
		\param	object		inserted object.
		\return	index or negative value if insertion failed.
	*/
	int Insert(I_DWORD position, const Object* objectPtr = NULL);
	/**
		Find element in queue using its position.
	*/
	int FindByPosition(I_DWORD position, int tolerance) const;

	I_DWORD GetPositionAt(int index) const;
	const Object& GetObjectAt(int index) const;
	Object& GetObjectAt(int index);

	/**
		Get front position.
	*/
	I_DWORD GetBackPosition() const;

	/**
		Get object at the front.
	*/
	const Object& GetBackObject() const;

	/**
		Get object at the front.
	*/
	Object& GetBackObject();

	int GetBackIndex() const;
	int GetNextFrontIndex() const;

	/**
		Remove front position.
	*/
	void PopBack();

	// static methods
	static int GetPrevIndex(int fifoIndex);
	static int GetNextIndex(int fifoIndex);

protected:
	struct Element
	{
		I_DWORD position;
		Object object;
	};

private:
	Element m_elements[MAX_SIZE];

	int m_nextFrontIndex;
	int m_backIndex;
};


// inline methods

template <class Object, int SizeBits>
inline bool TOrderedPositionsQueue<Object, SizeBits>::IsEmpty() const
{
	return m_backIndex == m_nextFrontIndex;
}


template <class Object, int SizeBits>
inline void TOrderedPositionsQueue<Object, SizeBits>::Reset()
{
	m_nextFrontIndex = 0;
	m_backIndex = 0;
}


template <class Object, int SizeBits>
inline I_DWORD TOrderedPositionsQueue<Object, SizeBits>::GetPositionAt(int index) const
{
	return m_elements[index].position;
}


template <class Object, int SizeBits>
inline const Object& TOrderedPositionsQueue<Object, SizeBits>::GetObjectAt(int index) const
{
	return m_elements[index].object;
}


template <class Object, int SizeBits>
inline Object& TOrderedPositionsQueue<Object, SizeBits>::GetObjectAt(int index)
{
	return m_elements[index].object;
}


template <class Object, int SizeBits>
inline I_DWORD TOrderedPositionsQueue<Object, SizeBits>::GetBackPosition() const
{
	I_ASSERT(!IsEmpty());

	return m_elements[m_backIndex].position;
}


template <class Object, int SizeBits>
const Object& TOrderedPositionsQueue<Object, SizeBits>::GetBackObject() const
{
	I_ASSERT(!IsEmpty());

	return m_elements[m_backIndex].object;
}


template <class Object, int SizeBits>
Object& TOrderedPositionsQueue<Object, SizeBits>::GetBackObject()
{
	I_ASSERT(!IsEmpty());

	return m_elements[m_backIndex].object;
}


template <class Object, int SizeBits>
inline int TOrderedPositionsQueue<Object, SizeBits>::GetBackIndex() const
{
	return m_backIndex;
}


template <class Object, int SizeBits>
inline int TOrderedPositionsQueue<Object, SizeBits>::GetNextFrontIndex() const
{
	return m_nextFrontIndex;
}


template <class Object, int SizeBits>
inline void TOrderedPositionsQueue<Object, SizeBits>::PopBack()
{
	I_ASSERT(!IsEmpty());

	m_backIndex = GetNextIndex(m_backIndex);
}


// static methods

template <class Object, int SizeBits>
inline int TOrderedPositionsQueue<Object, SizeBits>::GetPrevIndex(int fifoIndex)
{
	return (fifoIndex - 1) & INDEX_MASK;
}


template <class Object, int SizeBits>
inline int TOrderedPositionsQueue<Object, SizeBits>::GetNextIndex(int fifoIndex)
{
	return (fifoIndex + 1) & INDEX_MASK;
}


// public non-inline methods

template <class Object, int SizeBits>
int TOrderedPositionsQueue<Object, SizeBits>::Insert(I_DWORD position, const Object* objectPtr)
{
	int nextIndex = GetNextIndex(m_nextFrontIndex);

	if (nextIndex != m_backIndex){
		int insertionIndex = m_nextFrontIndex;
		while (insertionIndex != m_backIndex){	// shift all elements to assume correct elements order
			int prevInsertionIndex = GetPrevIndex(insertionIndex);

			if (I_SDWORD(m_elements[prevInsertionIndex].position - position) <= 0){
				break;
			}

			m_elements[insertionIndex] = m_elements[prevInsertionIndex];

			insertionIndex = prevInsertionIndex;
		}

		m_elements[insertionIndex].position = position;

		if (objectPtr != NULL){
			m_elements[insertionIndex].object = *objectPtr;
		}

		m_nextFrontIndex = nextIndex;

		return insertionIndex;
	}

	return -1;
}


template <class Object, int SizeBits>
int TOrderedPositionsQueue<Object, SizeBits>::FindByPosition(I_DWORD position, int tolerance) const
{
	for (int fifoIndex = m_backIndex; fifoIndex != m_nextFrontIndex; fifoIndex = GetNextIndex(fifoIndex)){
		I_SDWORD posDiff = I_SDWORD(position - m_elements[fifoIndex].position);
		if (posDiff < -I_SDWORD(tolerance)){	// each iteration is this difference smaller, you can cancel this search if it is true.
			break;
		}

		if (posDiff <= I_SDWORD(tolerance)){
			return fifoIndex;
		}
	}

	return -1;
}


} // namespace ilolv


#endif // !ilolv_TOrderedPositionsQueue_included


