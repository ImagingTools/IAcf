#include "iipr/CFeaturesContainer.h"


namespace iipr
{


// reimplemented (iipr::IFeaturesConsumer)

void CFeaturesContainer::InitFeatureSession(const IFeaturesSession* infoPtr, bool releaseFlag)
{
	istd::CChangeNotifier changePtr(this);

	m_featuresList.Reset();

	m_infoPtr.SetPtr(infoPtr, releaseFlag);
}


bool CFeaturesContainer::AddFeature(const iipr::IFeature* featurePtr, bool* /*isFullPtr*/)
{
	if (featurePtr != NULL){
		istd::CChangeNotifier changePtr(this);

		m_featuresList.PushBack(const_cast<iipr::IFeature*>(featurePtr));
		
		return true;
	}

	return false;
}


// reimplemented (iipr::IFeaturesContainer)

const IFeaturesSession* CFeaturesContainer::GetSessionInfo() const
{
	return m_infoPtr.GetPtr();
}


iipr::IFeaturesContainer::Features CFeaturesContainer::GetFeatures() const
{
	iipr::IFeaturesContainer::Features featuresList;
	
	for (int index = 0; index < m_featuresList.GetCount(); index++){
		featuresList.push_back(m_featuresList.GetAt(index));
	}

	return featuresList;
}


} // namespace iipr


