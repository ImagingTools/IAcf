#include <iocv/COcvUndistortImageProcessorComp.h>


// OpenCV includes
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

// ACF includes
#include <iprm/TParamsPtr.h>

// IACF includes
#include <iocv/COcvImage.h>
#include <iocv/COcvIntrinsicCameraCalibration.h>


namespace iocv
{


// protected methods

// reimplemented (iipr::CImageProcessorCompBase)

bool COcvUndistortImageProcessorComp::ProcessImage(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage) const
{
	try{
		iprm::TParamsPtr<i2d::ICalibration2d> cameraCalibrationParamPtr(paramsPtr, m_cameraCalibrationParamIdArrtPtr, m_defaultCameraCalibrationCompPtr, false);
		if (!cameraCalibrationParamPtr.IsValid()){
			outputImage.CopyFrom(inputImage);

			return true;
		}

		double alfa = 0.0;
		if (m_alfaScaleArrtPtr.IsValid()){
			alfa = *m_alfaScaleArrtPtr;
		}

		if (alfa < 0.0){
			alfa = 0.0;
		}

		if (alfa > 1.0){
			alfa = 1.0;
		}

		cv::Mat cvInput;
		COcvImage::ConvertFromBitmap(inputImage, cvInput);
		cv::Size imageSize;
		imageSize.width = inputImage.GetImageSize().GetX();
		imageSize.height = inputImage.GetImageSize().GetY();

		const iocv::COcvIntrinsicCameraCalibration* calibrationImplPtr = dynamic_cast<const iocv::COcvIntrinsicCameraCalibration*>(cameraCalibrationParamPtr.GetPtr());
		if (calibrationImplPtr != NULL){
			cv::Mat newMat = cv::getOptimalNewCameraMatrix(calibrationImplPtr->GetCameraMatrix(), calibrationImplPtr->GetDistorsion(), imageSize, alfa);

			cv::Mat cvOutput;
			cv::undistort(cvInput, cvOutput, calibrationImplPtr->GetCameraMatrix(), calibrationImplPtr->GetDistorsion(), newMat);

			return COcvImage::ConvertToBitmap(cvOutput, outputImage);
		}
	}
	catch (const cv::Exception& e){
		SendCriticalMessage(0, e.what());

		return false;
	}
	catch (...){
		SendCriticalMessage(0, QObject::tr("Unknown exception"));

		return false;
	}

	return false;
}


} // namespace iipr


