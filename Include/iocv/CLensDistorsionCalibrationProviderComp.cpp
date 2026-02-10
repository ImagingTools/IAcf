// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-IAcf-Commercial
#include <iocv/CLensDistorsionCalibrationProviderComp.h>


namespace iocv
{


// public methods

const i2d::ICalibration2d * CLensDistorsionCalibrationProviderComp::GetCalibration() const
{
	return GetWorkProduct();
}


// protected methods

iinsp::ISupplier::WorkStatus CLensDistorsionCalibrationProviderComp::ProduceObject(ProductType & result) const
{
	if (!m_ocvCalibrationCompPtr.IsValid()){
		return WS_FAILED;
	}

	CLensDistorsionCalibration calib(*m_ocvCalibrationCompPtr);
	result = calib;

	return WS_OK;
}


} // namespace iocv


