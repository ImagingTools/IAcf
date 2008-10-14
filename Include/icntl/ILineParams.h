#ifndef icntl_ILineParams_included
#define icntl_ILineParams_included


// ACF includes
#include "iser/ISerializable.h"

#include "icntl/icntl.h"


namespace icntl
{


class IInspectionUnitParams;
class IEjectorParams;


class ILineParams: public iser::ISerializable
{
public:
	/**
		Get number of inspection units.
	*/
	virtual int GetInspectionUnitsCount() const = 0;
	/**
		Get access to parameter of single inspection unit.
	*/
	virtual IInspectionUnitParams& GetInspectionUnitParams(int unitIndex) const = 0;
	/**
		Get number of ejectors.
	*/
	virtual int GetEjectorsCount() const = 0;
	/**
		Get access to parameter of single ejector.
	*/
	virtual IEjectorParams& GetEjectorParams(int ejectorIndex) const = 0;
	/**
		Get proportion between encoder ticks and physical units.
	*/
	virtual double GetTicksPerDistanceUnit() const = 0;
	/**
		Set proportion between encoder ticks and physical units.
	*/
	virtual void SetTicksPerDistanceUnit(double value) = 0;
	/**
		Get minimal object size.
		This value is in physical units.
	*/
	virtual double GetMinObjectSize() const = 0;
	/**
		Set minimal object size.
		This value is in physical units.
	*/
	virtual void SetMinObjectSize(double value) = 0;
	/**
		Get tolerance of position used to associate signals from few light barriers to single object.
		This value is in physical units.
	*/
	virtual double GetPositionTolerance() const = 0;
	/**
		Set tolerance of position used to associate signals from few light barriers to single object.
		This value is in physical units.
	*/
	virtual void SetPositionTolerance(double value) = 0;
	/**
		Check if ejection control is enabled.
	*/
	virtual bool IsEjectionControlEnabled() const = 0;
	/**
		Set ejection control to be enabled or disabled.
	*/
	virtual void SetEjectionControlEnabled(bool state = true) = 0;
	/**
		Get number of light barriers.
	*/
	virtual int GetLightBarriersCount() const = 0;
	/**
		Get position of light barrier.
		This value is in physical units relative to abstract base position.
	*/
	virtual double GetLightBarrierPosition(int barrierIndex) const = 0;
	/**
		Set position of light barrier.
		This value is in physical units relative to abstract base position.
	*/
	virtual void SetLightBarrierPosition(int barrierIndex, double position) = 0;
};


} // namespace icntl


#endif // !icntl_ILineParams_included


