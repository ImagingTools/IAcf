#ifndef icntl_IObjectInspection_included
#define icntl_IObjectInspection_included


// ACF includes
#include "istd/IPolymorphic.h"
#include "isys/ITimer.h"


namespace icntl
{


/**
	Represent single inspection session for one unit and one object.
*/
class IObjectInspection: virtual public istd::IPolymorphic
{
public:
	/**
		Get timestamp of this inspection.
		Typically camera trigger timestamp is taken.
	*/
	virtual const isys::ITimer& GetTimestamp() const = 0;
	/**
		Set ejector should be used for this object.
	*/
	virtual bool SetEjector(int ejectorIndex) = 0;
};


} // namespace icntl


#endif // !icntl_IObjectInspection_included


