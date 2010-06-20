#ifndef iipr_CFeaturesContainer_included
#define iipr_CFeaturesContainer_included


#include "istd/TPointerVector.h"
#include "istd/TChangeNotifier.h"

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
	virtual void ResetFeatures();
	virtual bool AddFeature(const iipr::IFeature* featurePtr, bool* isFullPtr = NULL);

	// reimplemented (iipr::IFeaturesContainer)
	virtual Features GetFeatures() const;

private:
	typedef istd::TPointerVector<iipr::IFeature> FeaturesList;

	FeaturesList m_featuresList;
};


} // namespace iipr


#endif // !iipr_CFeaturesContainer_included

