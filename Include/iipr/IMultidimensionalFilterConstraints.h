#ifndef iipr_IMultidimensionalFilterConstraints_included
#define iipr_IMultidimensionalFilterConstraints_included


// ACF includes
#include "istd/IPolymorphic.h"

// ACF-Solutions includes
#include "imeas/IUnitInfo.h"

#include "iipr/iipr.h"


namespace iipr
{


class IMultidimensionalFilterConstraints: virtual public istd::IPolymorphic
{
public:
	/**
		Get number of dimensions will be filtered.
	*/
	virtual int GetFilterDimensionsCount() const = 0;

	/**
		Get description of some filter element.
	*/
	virtual istd::CString GetFilterDescription(int dimension) const = 0;

	/**
		Get range of possible filter value for specified dimension.
	*/
	virtual const imeas::IUnitInfo& GetFilterUnitInfo(int dimension) const = 0;
};


} // namespace iipr


#endif // !iipr_IMultidimensionalFilterConstraints_included


