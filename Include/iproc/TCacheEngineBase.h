#ifndef iproc_TCacheEngineBase_included
#define iproc_TCacheEngineBase_included


namespace iproc
{


template <class Key, class CacheObject, class SourceObject>
class TCacheEngineBase: public TICacheEngine<Key, CacheObject>
{
public:
	double GetMaxCumulatedWeight() const;
	void SetMaxCumulatedWeight(double value);

	// reimplemented (iproc::TICacheEngine)
	virtual const CacheObject* LockCacheObject(const Key& key);
	virtual void UnlockCacheObject(const CacheObject* objectPtr);

protected:
	/**
		Remove elements from list if cumulated weight is above defined maximum.
	*/
	void CleanElementList();

	// abstract methods
	/**
		Calculate cache object from source object.
		\return	positive weight value if succeded of negative value (-1) if failed.
	*/
	virtual double CalcCacheObject(const SourceObject& source, CacheObject& cache) const = 0;
	virtual const SourceObject* LockSourceObject(const Key& key) = 0;
	virtual void UnlockSourceObject(const SourceObject* objectPtr) = 0;

private:
	double m_maxCumulatedWeight;

	mutable double m_cumulatedWeight;

	struct ListElement: public Key
	{
		ListElement()
		{
			lockedCount = 0;
			weight = 0.0;
		}

		double weight;
		CacheObject object;
		int lockedCount;
	};

	typedef std::list<ListElement> CachedList;

	CachedList m_cachedList;
};


// public methods


template <class Key, class CacheObject, class SourceObject>
double TCacheEngineBase<Key, CacheObject, SourceObject>::GetMaxCumulatedWeight() const
{
	return m_maxCumulatedWeight;
}


template <class Key, class CacheObject, class SourceObject>
void TCacheEngineBase<Key, CacheObject, SourceObject>::SetMaxCumulatedWeight(double value)
{
	m_maxCumulatedWeight = value;

	CleanElementList();
}


// reimplemented (iproc::TICacheEngine)

template <class Key, class CacheObject, class SourceObject>
const CacheObject* TCacheEngineBase<Key, CacheObject, SourceObject>::LockCacheObject(const Key& key)
{
	CachedList::iterator foundIter = std::find(m_cachedList.begin(), m_cachedList.end(), key);
	if (foundIter != m_cachedList.end()){
		foundIter->lockedCount++;

		return &foundIter->object;
	}

	const SourceObject* sourcePtr = LockSourceObject(key);
	if (sourcePtr != NULL){
		m_cachedList.push_back(ListElement());

		ListElement& element = m_cachedList.last();

		element.weight = CalcCacheObject(*sourcePtr, element.object);

		UnlockSourceObject(key);

		if (element.weight < 0){
			m_cachedList.pop_back();

			return NULL;
		}

		m_cumulatedWeight += element.weight;
		element.lockedCount++;

		CleanElementList();
		
		return &element.object;
	}

	return NULL;
}


template <class Key, class CacheObject, class SourceObject>
void TCacheEngineBase<Key, CacheObject, SourceObject>::UnlockCacheObject(const Key& key)
{
	CachedList::iterator foundIter = std::find(m_cachedList.begin(), m_cachedList.end(), key);
	I_ASSERT(foundIter != m_cachedList.end());	// if locked is done correctly, this element must exist.

	foundIter->lockedCount--;
}


// protected methods

template <class Key, class CacheObject, class SourceObject>
void TCacheEngineBase<Key, CacheObject, SourceObject>::CleanElementList()
{
	CachedList::iterator iter = m_cachedList.begin();
	while (		(m_cumulatedWeight > m_maxCumulatedWeight) &&
				(iter != m_cachedList.end())){
		if (iter->lockedCount <= 0){
			iter = m_cachedList.erase(iter);
		}
		else{
			++iter;
		}
	}
}


} // namespace iproc


#endif // !iproc_TCacheEngineBase_included


