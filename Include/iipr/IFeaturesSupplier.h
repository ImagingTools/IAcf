#ifndef iipr_IFeaturesSupplier_included
#define iipr_IFeaturesSupplier_included


#include "iproc/ISupplier.h"

#include "iipr/IFeaturesContainer.h"


namespace iipr
{


class IFeaturesSupplier: virtual public iproc::ISupplier
{
public:
	/**
		Get access to produced features container.
		\return	pointer to the feature container if this was correctly produced, or NULL.
	*/
	virtual const iipr::IFeaturesContainer* GetFeatures() const = 0;
};


} // namespace iipr


#endif // !iipr_IFeaturesSupplier_included

