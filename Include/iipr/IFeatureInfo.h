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
class IFeatureInfo: virtual public istd::IPolymorphic
{
public:
	/**
		Get unify ID of feature type.
	*/
	virtual int GetFeatureTypeId() const = 0;

	/**
		Get human readable description of feature type.
	*/
	virtual const istd::CString& GetFeatureTypeDescription() const = 0;

	/**
		Get human readable information single feature.
	*/
	virtual istd::CString GetFeatureDescription(const IFeature& feature) const = 0;
};


} // namespace iipr


#endif // !iipr_IFeatureInfo_included


