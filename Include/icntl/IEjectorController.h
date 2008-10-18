#ifndef icntl_IEjectorController_included
#define icntl_IEjectorController_included


// ACF includes
#include "istd/IPolymorphic.h"

#include "icntl/icntl.h"


namespace icntl
{


/**
	Controlling of single ejector.
*/
class IEjectorController: virtual public istd::IPolymorphic
{
public:
	/**
		Get number of ejected objects.
	*/
	virtual int GetEjectedCounter() const = 0;
	/**
		Provide a test ejection.
	*/
	virtual bool DoTestEjection() = 0;
};


} // namespace icntl


#endif // !icntl_IEjectorController_included


