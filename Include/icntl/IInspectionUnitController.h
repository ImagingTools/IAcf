#ifndef icntl_IInspectionUnitController_included
#define icntl_IInspectionUnitController_included


// ACF includes
#include "istd/IPolymorphic.h"
#include "isys/ITimer.h"

#include "icntl/icntl.h"


namespace icntl
{


class IObjectInspection;


/**
	Parameters of single inspection unit.
*/
class IInspectionUnitController: virtual public istd::IPolymorphic
{
public:
	/**
		Get next inspection object and remove it from waiting to inspection queue.
		\return	inspection object or NULL, if no object is waiting for inspection.
				Please note, that returned instance is owned by caller.
	*/
	virtual IObjectInspection* PopObjectInspection() = 0;
	/**
		Do camera trigger for this inspection unit.
		\return	pointer to timer object representing time point of trigger or NULL if trigger was impossible.
				Please note, that returned instance is owned by caller.
	*/
	virtual isys::ITimer* DoCameraTrigger() = 0;
};


} // namespace icntl


#endif // !icntl_IInspectionUnitController_included


