#include <iocv/CLensDistorsionCalibrationProviderComp.h>


namespace iocv
{


// public methods

const i2d::ICalibration2d * CLensDistorsionCalibrationProviderComp::GetCalibration() const
{
	return GetWorkProduct();
}


// protected methods

int CLensDistorsionCalibrationProviderComp::ProduceObject(ProductType & result) const
{
	if (!m_ocvCalibrationCompPtr.IsValid()) {
		return WS_FAILED;
	}

	CLensDistorsionCalibration calib(*m_ocvCalibrationCompPtr);
	result = calib;

	return WS_OK;
}


} // namespace iocv


