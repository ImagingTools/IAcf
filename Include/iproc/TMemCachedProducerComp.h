#ifndef iproc_TMemCachedProducerComp_included
#define iproc_TMemCachedProducerComp_included


// STL includes
#include <list>

#include "iproc/TILockedProducer.h"


namespace iproc
{


/**
	Template implementation iproc::TILockedProducer buffering objects in memory cache.
	As a source of produced object instances some slave implementation is used.
*/
template <class Key, class CacheObject>
class TMemCachedProducerComp:
			public icomp::CComponentBase,
			virtual public TILockedProducer<Key, CacheObject>
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef TILockedProducer<Key, CacheObject> LockedProducerType;

	I_BEGIN_BASE_COMPONENT(TMemCachedProducerComp);
		I_REGISTER_INTERFACE(LockedProducerType);
		I_ASSIGN(m_slaveCacheEngineCompPtr, "SlaveCacheEngine", "Slave cache engine providing access to cached object", true, "SlaveCacheEngine");
		I_ASSIGN(m_maxCachedObjectsAttrPtr, "MaxCachedObjects", "Maximal number of cached objects", true, 20);
	I_END_COMPONENT;

	// reimplemented (iproc::TILockedProducer)
	virtual const CacheObject* ProduceLockedObject(const Key& key);
	virtual void UnlockObject(const CacheObject* objectPtr);

protected:
	/**
		Remove elements from list if cumulated weight is above defined maximum.
	*/
	void CleanElementList();

private:
	I_REF(TILockedProducer<Key, CacheObject>, m_slaveCacheEngineCompPtr);
	I_ATTR(int, m_maxCachedObjectsAttrPtr);

	struct ListElement: public Key
	{
		ListElement()
		{
			lockedCount = 0;
		}

		CacheObject* objectPtr;
		int lockedCount;
	};

	typedef std::list<ListElement> CachedList;
	typedef std::map<CacheObject*, CachedList::iterator> ObjectToListMap;

	CachedList m_cachedList;
	ObjectToListMap m_objectToListMap;
};


// public methods

// reimplemented (iproc::TILockedProducer)

template <class Key, class CacheObject>
const CacheObject* TMemCachedProducerComp<Key, CacheObject>::ProduceLockedObject(const Key& key)
{
	CachedList::iterator foundIter = std::find(m_cachedList.begin(), m_cachedList.end(), key);
	if (foundIter != m_cachedList.end()){
		foundIter->lockedCount++;

		return foundIter->objectPtr;
	}

	if (m_slaveCacheEngineCompPtr.IsValid()){
		const CacheObject* objectPtr = m_slaveCacheEngineCompPtr->ProduceLockedObject(key);
		if (objectPtr != NULL){
			m_cachedList.push_back(ListElement());

			ListElement& element = m_cachedList.back();
			m_objectToListMap[objectPtr] = m_cachedList.last();

			element.objectPtr = objectPtr;
			element.lockedCount = 1;

			CleanElementList();
			
			return objectPtr;
		}
	}

	return NULL;
}


template <class Key, class CacheObject>
void TMemCachedProducerComp<Key, CacheObject>::UnlockObject(const CacheObject* objectPtr)
{
	ObjectToListMap::iterator foundIter = m_objectToListMap.find(objectPtr);
	I_ASSERT(foundIter != m_objectToListMap.end());	// if locked is done correctly, this element must exist.

	foundIter->second->lockedCount--;

	CleanElementList();
}


// protected methods

template <class Key, class CacheObject>
void TMemCachedProducerComp<Key, CacheObject>::CleanElementList()
{
	int maxCachedObjects = istd::Max(0, *m_maxCachedObjectsAttrPtr);

	CachedList::iterator iter = m_cachedList.begin();
	while (		(m_cachedList.size() > maxCachedObjects) &&
				(iter != m_cachedList.end())){
		I_ASSERT(m_objectToListMap.find(iter->objectPtr) != m_objectToListMap.end());	// object is present in objects map
		I_ASSERT(m_objectToListMap[iter->objectPtr] == iter);	// iterator stored for this object is correct

		if (iter->lockedCount <= 0){
			I_ASSERT(m_slaveCacheEngineCompPtr.IsValid());

			m_slaveCacheEngineCompPtr->UnlockObject(iter->objectPtr);

			m_objectToListMap.erase(iter->objectPtr);
			iter = m_cachedList.erase(iter);
		}
		else{
			++iter;
		}
	}

	I_ASSERT(m_cachedList.size() == m_objectToListMap.size());	// this both structures are coupled, the number of elements must be the same
}


} // namespace iproc


#endif // !iproc_TMemCachedProducerComp_included


