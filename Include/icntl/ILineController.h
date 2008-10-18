#ifndef icntl_ILineController_included
#define icntl_ILineController_included


// ACF includes
#include "istd/IPolymorphic.h"

#include "icntl/icntl.h"


namespace icntl
{


class IInspectionUnitController;
class IEjectorController;


/**
	Interface to control single production line.
*/
class ILineController: virtual public istd::IPolymorphic
{
public:
	/**
		Get number of inspection units.
	*/
	virtual int GetInspectionUnitsCount() const = 0;
	/**
		Get access to parameter of single inspection unit.
	*/
	virtual IInspectionUnitController& GetInspectionUnitController(int unitIndex) const = 0;
	/**
		Get number of ejectors.
	*/
	virtual int GetEjectorsCount() const = 0;
	/**
		Get access to parameter of single ejector.
	*/
	virtual IEjectorController& GetEjectorController(int ejectorIndex) const = 0;
	/**
		Get position of transmission belt.
	*/
	virtual bool GetTransmissionPosition(double& result) const = 0;
	/**
		Get position of object on transmission belt.
		\param	objectId	ID of object.
		\param	result		result object, will be set to position on transmission belt, if position is known.
		\return				true if object position is known.
	*/
	virtual bool GetObjectPosition(I_DWORD objectId, double& result) const = 0;
	/**
		Get ID of last object.
	*/
	virtual bool GetLastObjectId(I_DWORD& result) const = 0;
};


} // namespace icntl


#endif // !icntl_ILineController_included


