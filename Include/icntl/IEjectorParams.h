#ifndef icntl_IEjectorParams_included
#define icntl_IEjectorParams_included


// ACF includes
#include "istd/INamed.h"
#include "istd/IEnableable.h"
#include "iser/ISerializable.h"

#include "icntl/icntl.h"


namespace icntl
{


/**
	Parameters of single ejector.
*/
class IEjectorParams:
			virtual public iser::ISerializable,
			virtual public istd::INamed,
			virtual public istd::IEnableable
{
public:
	/**
		Get position of this ejector.
		This value is in physical units relative to abstract base position.
	*/
	virtual double GetEjectorPosition() const = 0;
	/**
		Set position of this ejector.
		This value is in physical units relative to abstract base position.
	*/
	virtual void SetEjectorPosition(double position) = 0;

	/**
		Get ejection distance.
		This value is in physical units relative to abstract base position.
	*/
	virtual double GetEjectionDistance() const = 0;
	/**
		Set ejection distance.
		This value is in physical units relative to abstract base position.
	*/
	virtual void SetEjectionDistance(double position) = 0;
};


} // namespace icntl


#endif // !icntl_IEjectorParams_included


