#ifndef iipr_CFeaturesContainer_included
#define iipr_CFeaturesContainer_included


#include "istd/TOptDelPtr.h"
#include "istd/TPointerVector.h"
#include "istd/TChangeNotifier.h"

#include "iipr/IFeaturesSession.h"
#include "iipr/IFeaturesContainer.h"


namespace iipr
{


/**
	Basic implementation of a features container.
*/
class CFeaturesContainer: public IFeaturesContainer
{
public:
	// reimplemented (iipr::IFeaturesConsumer)
	virtual void InitFeatureSession(const IFeaturesSession* infoPtr, bool releaseFlag);
	virtual bool AddFeature(const iipr::IFeature* featurePtr, bool* isFullPtr = NULL);

	// reimplemented (iipr::IFeaturesContainer)
	virtual const IFeaturesSession* GetSessionInfo() const;
	virtual iipr::IFeaturesContainer::Features GetFeatures() const;

private:
	istd::TOptDelPtr<const IFeaturesSession> m_infoPtr;

	typedef istd::TPointerVector<iipr::IFeature> FeaturesList;
	FeaturesList m_featuresList;
};


} // namespace iipr


#endif // !iipr_CFeaturesContainer_included

