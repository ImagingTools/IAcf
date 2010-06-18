#ifndef iipr_IFeature_included
#define iipr_IFeature_included


#include "iser/ISerializable.h"

#include "imath/TVarVector.h"

#include "iipr/iipr.h"


namespace iipr
{


class IFeatureInfo;


class IFeature: virtual public iser::ISerializable
{
public:
	/**
		Get access to feature information.
	*/
	virtual const IFeatureInfo* GetFeatureInfo() const = 0;
	/**
		Get weigth of this feature.
	*/
	virtual double GetWeight() const = 0;

	/**
		Get value of this feature.
		Interpretation of this value is depending on feature type.
	*/
	virtual imath::CVarVector GetValue() const = 0;
};


} // namespace iipr


#endif // !iipr_IFeature_included


