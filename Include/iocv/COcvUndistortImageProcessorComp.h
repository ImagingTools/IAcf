#pragma once


// ACF includes
#include <i2d/ICalibration2d.h>
#include <iipr/CImageProcessorCompBase.h>


namespace iocv
{


/**
	Processor used for removing the image distorsions caused by optical system according to the given camera calibration.
*/
class COcvUndistortImageProcessorComp : public iipr::CImageProcessorCompBase
{
public:
	typedef iipr::CImageProcessorCompBase BaseClass;

	I_BEGIN_COMPONENT(COcvUndistortImageProcessorComp);
		I_ASSIGN(m_cameraCalibrationParamIdAttrPtr, "CameraCalibrationParamId", "Parameter-ID of the calibration object in parameter set", false, "CameraCalibration");
		I_ASSIGN(m_defaultCameraCalibrationCompPtr, "DefaultCameraCalibration", "Default camera calibration", false, "DefaultCameraCalibration");
	I_END_COMPONENT;

protected:
	// reimplemented (CImageProcessorCompBase)
	virtual bool ProcessImage(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage) const override;

private:
	I_REF(i2d::ICalibration2d, m_defaultCameraCalibrationCompPtr);
	I_ATTR(QByteArray, m_cameraCalibrationParamIdAttrPtr);
};


} // namespace iocv



