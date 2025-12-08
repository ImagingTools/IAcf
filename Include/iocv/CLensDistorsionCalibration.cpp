#include <iocv/CLensDistorsionCalibration.h>


// ACF includes
#include <istd/CChangeNotifier.h>


namespace iocv
{


// public methods

CLensDistorsionCalibration::CLensDistorsionCalibration(const COcvIntrinsicCameraCalibration& calibration)
	:m_calibration(calibration)
{
}


// reimplemented (i2d::ICalibration2d)

const i2d::CRectangle* CLensDistorsionCalibration::GetArgumentArea() const
{
	return NULL;
}


const i2d::CRectangle* CLensDistorsionCalibration::GetResultArea() const
{
	return NULL;
}


const imath::IUnitInfo* CLensDistorsionCalibration::GetArgumentUnitInfo() const
{
	return NULL;
}


const imath::IUnitInfo* CLensDistorsionCalibration::GetResultUnitInfo() const
{
	return NULL;
}


istd::TUniqueInterfacePtr<i2d::ICalibration2d> CLensDistorsionCalibration::CreateCombinedCalibration(const ITransformation2d& /*transformation*/) const
{
	return NULL;
}


// reimplemented (i2d::ITransformation2d)

int CLensDistorsionCalibration::GetTransformationFlags() const
{
	return TF_FORWARD | TF_INVERTED | TF_INJECTIVE | TF_SURJECTIVE | TF_CONTINUES;
}


bool CLensDistorsionCalibration::GetDistance(
			const i2d::CVector2d& /*origPos1*/,
			const i2d::CVector2d& /*origPos2*/,
			double& /*result*/,
			ExactnessMode /*mode*/) const
{
	return false;
}


bool CLensDistorsionCalibration::GetPositionAt(
			const i2d::CVector2d& origPosition,
			i2d::CVector2d& result,
			ExactnessMode /*mode*/) const
{
	const cv::Matx33d& cameraMatrix = m_calibration.GetCameraMatrix();

	i2d::CVector2d orig = origPosition;

	orig.GetElementRef(0) -= cameraMatrix.col(2)(0);
	orig.GetElementRef(1) -= cameraMatrix.col(2)(1);
	orig.GetElementRef(0) /= cameraMatrix(0, 0);
	orig.GetElementRef(1) /= cameraMatrix(1, 1);
	m_calibration.GetPositionAt(orig, result);

	return true;
}


bool CLensDistorsionCalibration::GetInvPositionAt(
			const i2d::CVector2d& transfPosition,
			i2d::CVector2d& result,
			ExactnessMode /*mode*/) const
{
	m_calibration.GetInvPositionAt(transfPosition, result);

	const cv::Matx33d& cameraMatrix = m_calibration.GetCameraMatrix();

	result.GetElementRef(0) *= cameraMatrix(0, 0);
	result.GetElementRef(1) *= cameraMatrix(1, 1);
	result.GetElementRef(0) += cameraMatrix.col(2)(0);
	result.GetElementRef(1) += cameraMatrix.col(2)(1);

	return true;
}


bool CLensDistorsionCalibration::GetLocalTransform(
			const i2d::CVector2d& /*origPosition*/,
			i2d::CAffine2d& /*result*/,
			ExactnessMode /*mode*/) const
{
	return false;
}


bool CLensDistorsionCalibration::GetLocalInvTransform(
			const i2d::CVector2d& /*transfPosition*/,
			i2d::CAffine2d& /*result*/,
			ExactnessMode /*mode*/) const
{
	return false;
}


// reimplemented (imath::TISurjectFunction)

bool CLensDistorsionCalibration::GetInvValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	return GetInvPositionAt(argument, result);
}


i2d::CVector2d CLensDistorsionCalibration::GetInvValueAt(const i2d::CVector2d& argument) const
{
	i2d::CVector2d retVal;

	GetInvValueAt(argument, retVal);

	return retVal;
}


// reimplemented (imath::TIMathFunction)

bool CLensDistorsionCalibration::GetValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	return GetPositionAt(argument, result);
}


i2d::CVector2d CLensDistorsionCalibration::GetValueAt(const i2d::CVector2d& argument) const
{
	i2d::CVector2d retVal;

	GetValueAt(argument, retVal);

	return retVal;
}


// reimplemented (iser::ISerializable)

bool CLensDistorsionCalibration::Serialize(iser::IArchive& archive)
{
	return m_calibration.Serialize(archive);
}


// reimplemented (istd::IChangeable)

int CLensDistorsionCalibration::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE | SO_COMPARE | SO_RESET;
}


bool CLensDistorsionCalibration::CopyFrom(const istd::IChangeable& object, CompatibilityMode /*mode*/)
{
	const CLensDistorsionCalibration* calibrationImplPtr = dynamic_cast<const CLensDistorsionCalibration*>(&object);
	if (calibrationImplPtr != NULL){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		*this = *calibrationImplPtr;

		return true;
	}

	const COcvIntrinsicCameraCalibration* intrinsicCameraCalibrationPtr = dynamic_cast<const COcvIntrinsicCameraCalibration*>(&object);
	if (intrinsicCameraCalibrationPtr != NULL){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		CLensDistorsionCalibration calib(*intrinsicCameraCalibrationPtr);

		*this = calib;
		return true;
	}

	return false;
}


bool CLensDistorsionCalibration::IsEqual(const istd::IChangeable& object) const
{
	const CLensDistorsionCalibration* calibrationImplPtr = dynamic_cast<const CLensDistorsionCalibration*>(&object);
	if (calibrationImplPtr != NULL){
		return m_calibration.IsEqual(calibrationImplPtr->m_calibration);
	}

	return false;
}


istd::IChangeableUniquePtr CLensDistorsionCalibration::CloneMe(CompatibilityMode /*mode*/) const
{
	return new CLensDistorsionCalibration(*this);
}


bool CLensDistorsionCalibration::ResetData(CompatibilityMode /*mode*/)
{
	return m_calibration.ResetData();
}


} // namespace iocv


