#ifndef iipr_IFeatureInfo_included
#define iipr_IFeatureInfo_included


// ACF includes
#include "istd/IPolymorphic.h"
#include "istd/CString.h"

#include "iipr/iipr.h"


namespace iipr
{


class IFeature;


/**
	Provides information about some feature type.
*/
class IFeaturesSession: virtual public istd::IPolymorphic
{
public:
	enum FeatureElementId
	{
		FEI_IMAGE_POSITION = 23790,
		FEI_SIZE,
		FEI_SCALE,
		FEI_ANGLE,
		FEI_CALIPER_POSITION = 3243,
		FEI_CALIPER_POLARITY
	};

	/**
		Get unify ID of feature type.
	*/
	virtual int GetFeatureTypeId() const = 0;

	/**
		Get human readable description of feature type.
	*/
	virtual const istd::CString& GetFeatureTypeDescription() const = 0;
};


} // namespace iipr


#endif // !iipr_IFeatureInfo_included


