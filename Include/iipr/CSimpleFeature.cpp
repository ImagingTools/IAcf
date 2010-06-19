#include "iipr/CSimpleFeature.h"


namespace iipr
{


// public methods

CSimpleFeature::CSimpleFeature(double weight)
:	m_weight(weight)
{
}


void CSimpleFeature::SetValue(int elementId, const imath::CVarVector& value)
{
	m_valuesMap[elementId] = value;
}


void CSimpleFeature::InsertSlaveFeature(const IFeature* featurePtr)
{
	I_ASSERT(featurePtr != NULL);

	m_slaveFeatures.insert(featurePtr);
}


void CSimpleFeature::InsertSourceFeature(const IFeature* featurePtr)
{
	I_ASSERT(featurePtr != NULL);

	m_sourceFeatures.insert(featurePtr);
}


// reimplemented (iipr::IFeature)

double CSimpleFeature::GetWeight() const
{
	return m_weight;
}


const imath::CVarVector& CSimpleFeature::GetValue(int elementId) const
{
	ValuesMap::const_iterator foundIter = m_valuesMap.find(elementId);
	if (foundIter != m_valuesMap.begin()){
		return foundIter->second;
	}

	for (		Features::const_iterator slaveIter = m_slaveFeatures.begin();
				slaveIter != m_slaveFeatures.end();
				++slaveIter){
		I_ASSERT(*slaveIter != NULL);

		const IFeature* featurePtr = *slaveIter;

		const imath::CVarVector& slaveValue = featurePtr->GetValue(elementId);
		if (slaveValue.GetElementsCount() > 0){
			return slaveValue;
		}
	}

	static imath::CVarVector noValue;

	return noValue;
}


const imath::CVarVector& CSimpleFeature::GetLocalValue(int elementId) const
{
	ValuesMap::const_iterator foundIter = m_valuesMap.find(elementId);
	if (foundIter != m_valuesMap.begin()){
		return foundIter->second;
	}

	static imath::CVarVector noValue;

	return noValue;
}


IFeature::Features CSimpleFeature::GetSlaveFeatures() const
{
	return m_slaveFeatures;
}


IFeature::Features CSimpleFeature::GetSourceFeatures() const
{
	return m_sourceFeatures;
}


// reimplemented (iser::ISerializable)

bool CSimpleFeature::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag featureTag("Feature", "All feature elements");
	static iser::CArchiveTag elementTag("FeatureElement", "Single feature element");
	static iser::CArchiveTag elementIdTag("FeatureElId", "ID of feature element");

	if (archive.IsStoring()){
		int elementsCount = int(m_valuesMap.size());

		retVal = retVal && archive.BeginMultiTag(featureTag, elementTag, elementsCount);
		for (		ValuesMap::const_iterator elementIter = m_valuesMap.begin();
					elementIter != m_valuesMap.end();
					++elementIter){
			retVal = retVal && archive.BeginTag(elementTag);

			int elementId = elementIter->first;
			retVal = retVal && archive.BeginTag(elementIdTag);
			retVal = retVal && archive.Process(elementId);
			retVal = retVal && archive.EndTag(elementIdTag);

			imath::CVarVector elementValue = elementIter->second;
			retVal = retVal && elementValue.Serialize(archive);

			retVal = retVal && archive.EndTag(elementTag);
		}
		retVal = retVal && archive.EndTag(featureTag);
	}
	else{
		m_valuesMap.clear();

		int elementsCount = 0;
		retVal = retVal && archive.BeginMultiTag(featureTag, elementTag, elementsCount);
		if (!retVal){
			return false;
		}

		for (int i = 0; i < elementsCount; ++i){
			retVal = retVal && archive.BeginTag(elementTag);

			int elementId;
			retVal = retVal && archive.BeginTag(elementIdTag);
			retVal = retVal && archive.Process(elementId);
			retVal = retVal && archive.EndTag(elementIdTag);

			imath::CVarVector elementValue;
			retVal = retVal && elementValue.Serialize(archive);

			if (!retVal){
				return false;
			}

			m_valuesMap[elementId] = elementValue;

			retVal = retVal && archive.EndTag(elementTag);
		}
		retVal = retVal && archive.EndTag(featureTag);
	}

	return retVal;
}


} // namespace iipr

