#ifndef iproc_TFileCachedProducerCompBase_included
#define iproc_TFileCachedProducerCompBase_included


// ACF includes
#include "iser/IFileLoader.h"
#include "icomp/CComponentBase.h"

#include "iproc/TILockedProducer.h"


namespace iproc
{


/**
	Template base implementation iproc::TILockedProducer caching (buffering) objects using file system.
	Please note, that no memory caching is done - if you ask second time of existing (locked) object,
	second instance will be created and loaded.
*/
template <class Key, class CacheObject>
class TFileCachedProducerCompBase:
			public icomp::CComponentBase,
			virtual public TILockedProducer<Key, CacheObject>
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_BASE_COMPONENT(TFileCachedProducerCompBase);
		I_ASSIGN(m_cacheLoaderCompPtr, "CacheLoader", "Loads and saves cached object to temporary file", true, "CacheLoader");
		I_ASSIGN(m_slaveCacheEngineCompPtr, "SlaveCacheEngine", "Slave cache engine providing access to cached object", true, "SlaveCacheEngine");
		I_ASSIGN(m_maxCachedFilesAttrPtr, "MaxCachedFiles", "Maximal number of cached files", true, 10);
	I_END_COMPONENT;

	// reimplemented (iproc::TILockedProducer)
	virtual const CacheObject* ProduceLockedObject(const Key& key);
	virtual void UnlockObject(const CacheObject* objectPtr);

protected:
	/**
		Removes cache files if there is too many of them.
	*/
	void CleanFileList();

	// abstract methods
	/**
		Calculate unique file path of temporary file cache object.
	*/
	virtual istd::CString CalcCacheFilePath(const Key& key) const = 0;

private:
	I_REF(iser::IFileLoader, m_cacheLoaderCompPtr);
	I_REF(TILockedProducer<Key, CacheObject>, m_slaveCacheEngineCompPtr);
	I_ATTR(int, m_maxCachedFilesAttrPtr);

	typedef std::map<Key, istd::CString> KeyToFileNameMap;
	typedef std::list<Key> RecentlyUsedKeys;
	typedef std::set<CacheObject*> OwnedObjects;

	mutable KeyToFileNameMap m_keyToFileNameMap;
	mutable RecentlyUsedKeys m_recentlyUsedKeys;
	mutable OwnedObjects m_ownedObjects;
};


// public methods

// reimplemented (iproc::TILockedProducer)

template <class Key, class CacheObject>
const CacheObject* TFileCachedProducerCompBase<Key, CacheObject>::ProduceLockedObject(const Key& key)
{
	if (m_cacheLoaderCompPtr.IsValid()){
		KeyToFileNameMap::iterator foundIter = m_keyToFileNameMap.find(key);
		if (foundIter != m_keyToFileNameMap.end()){
			const istd::CString& cacheFilePath = foundIter->second;
			I_ASSERT(!cacheFilePath.IsEmpty());

			if (m_cacheLoaderCompPtr->IsOperationSupported(
						NULL,
						&cacheFilePath,
						iser::IFileLoader::QF_NO_SAVING | iser::IFileLoader::QF_NAMED_ONLY,
						false)){
				istd::TDelPtr<CacheObject> retVal(new CacheObject);

				if (m_cacheLoaderCompPtr->LoadFromFile(*retVal, cacheFilePath) != iser::IFileLoader::StateOk){
					m_ownedObjects.insert(retVal.GetPtr());

					return retVal.PopPtr();
				}
				else{
					m_keyToFileNameMap.erase(foundIter);
				}
			}
		}
	}

	if (m_slaveCacheEngineCompPtr.IsValid()){
		CacheObject* retVal = m_slaveCacheEngineCompPtr->ProduceLockedObject(key);
		if (retVal != NULL){
			istd::CString cacheFilePath = CalcCacheFilePath(key);
			if (!cacheFilePath.IsEmpty() && m_cacheLoaderCompPtr.IsValid()){
				if (m_cacheLoaderCompPtr->SaveToFile(retVal, cacheFilePath)){
					m_keyToFileNameMap[key] = cacheFilePath;
					m_recentlyUsedKeys.push_back(key);

					const_cast<CFileCachedInterpolator&>(*this).CleanFileList();

					return 1;
				}
			}
		}

		return retVal;
	}

	return NULL;
}


template <class Key, class CacheObject>
void TFileCachedProducerCompBase<Key, CacheObject>::UnlockObject(const CacheObject* objectPtr)
{
	if (		m_ownedObjects.find(objectPtr) == m_ownedObjects.end() &&
				m_slaveCacheEngineCompPtr.IsValid()){
		m_slaveCacheEngineCompPtr->UnlockObject(objectPtr);

		return;
	}

	delete objectPtr;
}


} // namespace iproc


#endif // !iproc_TFileCachedProducerCompBase_included


