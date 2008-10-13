#ifndef icntl_IInspectionUnitParams_included
#define icntl_IInspectionUnitParams_included


// ACF includes
#include "istd/INamed.h"
#include "istd/IEnableable.h"
#include "iser/ISerializable.h"

#include "icntl/icntl.h"


namespace icntl
{


/**
	Parameters of single inspection unit.
*/
class IInspectionUnitParams:
			virtual public iser::ISerializable,
			virtual public istd::INamed,
			virtual public istd::IEnableable
{
public:
	/**
		Get offset between light barrier trigger point and trigger position.
	*/
	virtual double GetTriggerOffset() const = 0;
	/**
		Set offset between light barrier trigger point and trigger position.
	*/
	virtual void SetTriggerOffset(double position) = 0;

	/**
		Set object position factor of light barrier trigger.
		If this is 0 object begin is mean, if it is 1 object end.
	*/
	virtual double GetTriggerAlpha() const = 0;
	/**
		Set object position factor of light barrier trigger.
		If this is 0 object begin is mean, if it is 1 object end.
	*/
	virtual void SetTriggerAlpha(double value) = 0;
};


} // namespace icntl


#endif // !icntl_IInspectionUnitParams_included


