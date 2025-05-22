#pragma once


// ACF includes
#include <i2d/ICalibration2d.h>
#include <iipr/CImageProcessorCompBase.h>
#include <iprm/IParamsManager.h>

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
		//special case
		I_ASSIGN(m_selectionParamIdAttrPtr, "SelectionId", "Id of the selection parameter, which defines paramsSet from Manager", false, "Index");
		I_ASSIGN(m_paramsManagerCompPtr, "ManagerToSelectFrom", "ParamsManager from which through SelectionId a paramsSet will be extracted", false, "Manager");
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
	I_ATTR(QByteArray, m_selectionParamIdAttrPtr);
	I_REF(iprm::IParamsManager, m_paramsManagerCompPtr);
};


} // namespace iocv



