#include <iocv/COcvIntrinsicCameraCalibration.h>


// OpenCV includes
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

// ACF includes
#include <istd/CChangeNotifier.h>


namespace iocv
{


// public methods

COcvIntrinsicCameraCalibration::COcvIntrinsicCameraCalibration()
{
	Reset();
}


const COcvIntrinsicCameraCalibration::CameraMatrix& COcvIntrinsicCameraCalibration::GetCameraMatrix() const
{
	return m_cameraMatrix;
}


void COcvIntrinsicCameraCalibration::SetCameraMatrix(const COcvIntrinsicCameraCalibration::CameraMatrix& cameraMatrix)
{
	istd::CChangeNotifier notifier(this);

	m_cameraMatrix = cameraMatrix;
}


const COcvIntrinsicCameraCalibration::DistorsionCoeff& COcvIntrinsicCameraCalibration::GetDistorsion() const
{
	return m_distorsionCoeff;
}


void COcvIntrinsicCameraCalibration::SetDistorsion(const COcvIntrinsicCameraCalibration::DistorsionCoeff& distorsionCoeff)
{
	istd::CChangeNotifier notifier(this);

	m_distorsionCoeff = distorsionCoeff;
}


// reimplemented (i2d::ICalibration2d)

const i2d::CRectangle* COcvIntrinsicCameraCalibration::GetArgumentArea() const
{
	return NULL;
}


const i2d::CRectangle* COcvIntrinsicCameraCalibration::GetResultArea() const
{
	return NULL;
}


const imath::IUnitInfo* COcvIntrinsicCameraCalibration::GetArgumentUnitInfo() const
{
	return NULL;
}


const imath::IUnitInfo* COcvIntrinsicCameraCalibration::GetResultUnitInfo() const
{
	return NULL;
}


const i2d::ICalibration2d* COcvIntrinsicCameraCalibration::CreateCombinedCalibration(const ITransformation2d& /*transformation*/) const
{
	return NULL;
}


// reimplemented (i2d::ITransformation2d)

int COcvIntrinsicCameraCalibration::GetTransformationFlags() const
{
	return TF_FORWARD | TF_INVERTED | TF_INJECTIVE | TF_SURJECTIVE | TF_CONTINUES;
}


bool COcvIntrinsicCameraCalibration::GetDistance(
			const i2d::CVector2d& /*origPos1*/,
			const i2d::CVector2d& /*origPos2*/,
			double& /*result*/,
			ExactnessMode /*mode*/) const
{
	return false;
}


bool COcvIntrinsicCameraCalibration::GetPositionAt(
			const i2d::CVector2d& origPosition,
			i2d::CVector2d& result,
			ExactnessMode /*mode*/) const
{
	std::vector<cv::Point3f>  input(1);
	input[0].x = origPosition.GetX();
	input[0].y = origPosition.GetY();
	input[0].z = 1.0;

	std::vector<cv::Point2f>  output(1);
	cv::Mat rvec = cv::Mat::zeros(3, 1, CV_32FC1);
	cv::Mat tvec = cv::Mat::zeros(3, 1, CV_32FC1);
	cv::projectPoints(input, rvec, tvec, m_cameraMatrix, m_distorsionCoeff, output);

	const cv::Point2f& out = output[0];
	result.SetX(out.x);
	result.SetY(out.y);

	return true;
}


bool COcvIntrinsicCameraCalibration::GetInvPositionAt(
			const i2d::CVector2d& transfPosition,
			i2d::CVector2d& result,
			ExactnessMode /*mode*/) const
{
	bool emptyDistortion = true;

	for (int i = 0; i < m_distorsionCoeff.rows; i++){
		if (std::abs(m_distorsionCoeff(i, 0)) > I_EPSILON){
			emptyDistortion = false;
			break;
		}
	}

	if (emptyDistortion){
		result = transfPosition;
		return true;
	}

	std::vector<cv::Point2f> input(1);
	input[0].x = transfPosition.GetX();
	input[0].y = transfPosition.GetY();
	std::vector<cv::Point2f> output(1);

	cv::undistortPoints(input, output, m_cameraMatrix, m_distorsionCoeff);

	cv::Point2f out = output[0];

	result.SetX(out.x);
	result.SetY(out.y);

	return true;
}


bool COcvIntrinsicCameraCalibration::GetLocalTransform(
			const i2d::CVector2d& /*origPosition*/,
			i2d::CAffine2d& /*result*/,
			ExactnessMode /*mode*/) const
{
	return false;
}


bool COcvIntrinsicCameraCalibration::GetLocalInvTransform(
			const i2d::CVector2d& /*transfPosition*/,
			i2d::CAffine2d& /*result*/,
			ExactnessMode /*mode*/) const
{
	return false;
}


// reimplemented (imath::TISurjectFunction)

bool COcvIntrinsicCameraCalibration::GetInvValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	return GetInvPositionAt(argument, result);
}


i2d::CVector2d COcvIntrinsicCameraCalibration::GetInvValueAt(const i2d::CVector2d& argument) const
{
	i2d::CVector2d retVal;
	
	GetInvValueAt(argument, retVal);

	return retVal;
}


// reimplemented (imath::TIMathFunction)

bool COcvIntrinsicCameraCalibration::GetValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	return GetPositionAt(argument, result);
}


i2d::CVector2d COcvIntrinsicCameraCalibration::GetValueAt(const i2d::CVector2d& argument) const
{
	i2d::CVector2d retVal;
	
	GetValueAt(argument, retVal);

	return retVal;
}
	
	
// reimplemented (iser::ISerializable)

bool COcvIntrinsicCameraCalibration::Serialize(iser::IArchive& archive)
{
	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	static const iser::CArchiveTag s_cameraMatrixTag("CameraMatrix", "Camera matrix coefficients", iser::CArchiveTag::TT_GROUP);
	static const iser::CArchiveTag s_distosionCoeffTag("DistorsionCoeff", "Lense distorsion coefficients", iser::CArchiveTag::TT_GROUP);

	imath::TMatrix<3, 3> cameraMatrix;
	imath::TVector<8> distorsionCoeff;

	if (isStoring){
		for (int i = 0; i < m_cameraMatrix.cols; ++i){
			for (int j = 0; j < m_cameraMatrix.rows; ++j){
				cameraMatrix.GetAtRef(i, j) = m_cameraMatrix(i, j);
			}
		}

		for (int i = 0; i < m_distorsionCoeff.rows; i++){
			distorsionCoeff.GetElementRef(i) = m_distorsionCoeff(i, 0);
		}
	}

	retVal = retVal && archive.BeginTag(s_cameraMatrixTag);
	retVal = retVal && cameraMatrix.Serialize(archive);
	retVal = retVal && archive.EndTag(s_cameraMatrixTag);

	retVal = retVal && archive.BeginTag(s_distosionCoeffTag);
	retVal = retVal && distorsionCoeff.Serialize(archive);
	retVal = retVal && archive.EndTag(s_distosionCoeffTag);

	if (!isStoring){
		for (int i = 0; i < m_cameraMatrix.cols; ++i){
			for (int j = 0; j < m_cameraMatrix.rows; ++j){
				m_cameraMatrix(i, j) = cameraMatrix.GetAt(i, j);
			}
		}

		for (int i = 0; i < m_distorsionCoeff.rows; i++){
			m_distorsionCoeff(i, 0) = distorsionCoeff.GetElement(i);
		}
	}

	return retVal;
}


// reimplemented (istd::IChangeable)

int COcvIntrinsicCameraCalibration::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE | SO_COMPARE | SO_RESET;
}


bool COcvIntrinsicCameraCalibration::CopyFrom(const istd::IChangeable& object, CompatibilityMode /*mode*/)
{
	const COcvIntrinsicCameraCalibration* calibrationImplPtr = dynamic_cast<const COcvIntrinsicCameraCalibration*>(&object);
	if (calibrationImplPtr != NULL){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		*this = *calibrationImplPtr;

		return true;
	}

	return false;
}


bool COcvIntrinsicCameraCalibration::IsEqual(const istd::IChangeable& object) const
{
	const COcvIntrinsicCameraCalibration* calibrationImplPtr = dynamic_cast<const COcvIntrinsicCameraCalibration*>(&object);
	if (calibrationImplPtr != NULL){
		if (m_cameraMatrix != calibrationImplPtr->m_cameraMatrix){
			return false;
		}

		if (m_distorsionCoeff != calibrationImplPtr->m_distorsionCoeff){
			return false;
		}

		return true;
	}

	return false;
}


istd::IChangeable* COcvIntrinsicCameraCalibration::CloneMe(CompatibilityMode /*mode*/) const
{
	return new COcvIntrinsicCameraCalibration(*this);
}


bool COcvIntrinsicCameraCalibration::ResetData(CompatibilityMode /*mode*/)
{
	Reset();

	return true;
}


// private methods

void COcvIntrinsicCameraCalibration::Reset()
{
	m_cameraMatrix.all(0.0);
	m_cameraMatrix(0, 0) = 1.0;
	m_cameraMatrix(1, 1) = 1.0;
	m_cameraMatrix(2, 2) = 1.0;
	m_distorsionCoeff.all(0.0);
}


} // namespace iocv


