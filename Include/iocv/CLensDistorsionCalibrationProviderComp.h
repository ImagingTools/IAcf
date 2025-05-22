#pragma once


// ACF includes
#include <i2d/ICalibrationProvider.h>

// ACF-Solutions includes
#include <iinsp/TSupplierCompWrap.h>

// IACF includes
#include <iocv/COcvIntrinsicCameraCalibration.h>
#include <iocv/CLensDistorsionCalibration.h>


namespace iocv
{

class CLensDistorsionCalibrationProviderComp:
			public iinsp::TSupplierCompWrap<CLensDistorsionCalibration>,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iinsp::TSupplierCompWrap<CLensDistorsionCalibration> BaseClass;

	I_BEGIN_COMPONENT(CLensDistorsionCalibrationProviderComp);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_calibrationCompPtr, "IntrinsicCameraCalibration", "Intrinsic camera calibration", true, "IntrinsicCameraCalibration");
		I_ASSIGN_TO(m_ocvCalibrationCompPtr, m_calibrationCompPtr, true);
	I_END_COMPONENT;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const override;

protected:
	// reimplemented (iinsp::TSupplierCompWrap)
	virtual WorkStatus ProduceObject(ProductType& result) const override;

private:
	I_REF(i2d::ICalibration2d, m_calibrationCompPtr);
	I_REF(iocv::COcvIntrinsicCameraCalibration, m_ocvCalibrationCompPtr);
};


} // namespace iocv


