#include <iocv/COcvIntrinsicCameraCalibrationSupplierComp.h>


// OpenCV includes
#include <opencv2/calib3d/calib3d.hpp>

// ACF includes
#include <iprm/TParamsPtr.h>
#include <icalib/CPointGrid.h>


// internal static methods

static std::vector<cv::Point3f> CalcBoardCornerPositions(cv::Size boardSize, float squareSize)
{
	std::vector<cv::Point3f> corners;

	for (int j = 0; j < boardSize.height; ++j){
		for (int i = 0; i < boardSize.width; ++i){
			corners.push_back(cv::Point3f(float(i*squareSize), float(j*squareSize), 0));
		}
	}

	return corners;
}


// internal grid point consumer

struct PointGridConsumer: public iipr::IFeaturesConsumer
{
	PointGridConsumer()
	{
		points.resize(1);
	}

	// reimplemented (iipr::IFeaturesConsumer)
	virtual void ResetFeatures() override
	{
		points[0].clear();
		gridSize.width = 0;
		gridSize.height = 0;
	}

	virtual bool AddFeature(const imeas::INumericValue* featurePtr, bool* /*isFullPtr = NULL*/) override
	{
		if (featurePtr != NULL){
			const icalib::CPointGrid* pointGridPtr = dynamic_cast<const icalib::CPointGrid*>(featurePtr);
			if (pointGridPtr != NULL){
				gridSize.width = pointGridPtr->GetSize(0);
				gridSize.height = pointGridPtr->GetSize(1);
				points[0].resize(gridSize.width*gridSize.height);

				int k = 0;
				for (int j = 0; j < gridSize.height; ++j){
					for (int i = 0; i < gridSize.width; ++i){
						istd::TIndex<2> ind;
						ind.SetAt(0, i);
						ind.SetAt(1, j);
						const i2d::CVector2d& v = pointGridPtr->GetAt(ind);
						points[0][k++] = cv::Point2f(v.GetX(), v.GetY());
					}
				}
			}

			delete featurePtr;
		}

		return true;
	}

	std::vector<std::vector<cv::Point2f>> points;
	cv::Size gridSize;
};


namespace iocv
{


// public methods

// reimplemented (i2d::ICalibrationProvider)

COcvIntrinsicCameraCalibrationSupplierComp::COcvIntrinsicCameraCalibrationSupplierComp()
{
	m_emptyCalibration.SetTransformation(i2d::CAffine2d::GetIdentity());
}


const i2d::ICalibration2d* COcvIntrinsicCameraCalibrationSupplierComp::GetCalibration() const
{
	return GetWorkProduct();
}


// protected methods

// reimplemented (iinsp::TSupplierCompWrap)

int COcvIntrinsicCameraCalibrationSupplierComp::ProduceObject(ProductType& result) const
{
	result.ResetData();

	try {
		if (!m_bitmapProviderCompPtr.IsValid()){
			SendErrorMessage(0, "Bitmap provider is undefined");

			return WS_FAILED;
		}

		const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
		if (bitmapPtr == NULL){
			SendErrorMessage(0, "Bitmap is undefined");

			return WS_FAILED;
		}

		if (!m_pointGridExtractorCompPtr.IsValid()){
			SendErrorMessage(0, "Points grid extractor is undefined");

			return WS_FAILED;
		}

		const iprm::IParamsSet* paramsPtr = GetModelParametersSet();

		PointGridConsumer imagePoints;
		if (m_pointGridExtractorCompPtr->DoExtractFeatures(paramsPtr, *bitmapPtr, imagePoints) != iproc::IProcessor::TS_OK){
			SendErrorMessage(0, "Can not extract of points grid");

			return WS_FAILED;
		}

		double cellSize = *m_defaultCellSizeAttrPtr;
		if (paramsPtr != NULL && m_cellSizeParamId.IsValid()){
			iprm::TParamsPtr<imeas::INumericValue> checkboardParamsPtr(paramsPtr, *m_cellSizeParamId);
			if (checkboardParamsPtr.IsValid()){
				imath::CVarVector params = checkboardParamsPtr->GetValues();
				if (params.GetElementsCount() >= 1){
					cellSize = int(params.GetElement(0));
				}
			}
		}

		std::vector<std::vector<cv::Point3f>> objectPoints(1);
		objectPoints[0] = CalcBoardCornerPositions(imagePoints.gridSize, cellSize);

		cv::Size imageSize(bitmapPtr->GetImageSize().GetX(), bitmapPtr->GetImageSize().GetY());
		cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
		cameraMatrix.at<double>(0, 0) = 1.0;
		cv::Mat distorsionCoeffs = cv::Mat::zeros(5, 1, CV_64F);
		std::vector<cv::Mat> rvecs; // extrinsic camera rotation matrix
		std::vector<cv::Mat> tvecs; // extrinsic camera translation

		int flags = cv::CALIB_FIX_ASPECT_RATIO;

		if (m_fixK2AttrPtr.IsValid() && *m_fixK2AttrPtr){
			flags |= cv::CALIB_FIX_K2;
		}

		if (m_fixK3AttrPtr.IsValid() && *m_fixK3AttrPtr){
			flags |= cv::CALIB_FIX_K3;
		}

		if (m_fixPrincipalPointAttrPtr.IsValid() && *m_fixPrincipalPointAttrPtr){
			flags |= cv::CALIB_FIX_PRINCIPAL_POINT;
		}

		//Find intrinsic and extrinsic camera parameters
		cv::calibrateCamera(
					objectPoints,
					imagePoints.points,
					imageSize,
					cameraMatrix,
					distorsionCoeffs,
					rvecs,
					tvecs,
					flags);

		if (!cv::checkRange(cameraMatrix) || !cv::checkRange(distorsionCoeffs)){
			SendErrorMessage(0, "Calibration was not found");

			return WS_FAILED;
		}

		result.SetCameraMatrix(cameraMatrix);
		result.SetDistorsion(distorsionCoeffs);

		m_lastCalibration = result;

		return WS_OK;
	}
	catch (const cv::Exception& e){
		SendCriticalMessage(0, e.what());

		m_lastCalibration.ResetData();
		return WS_FAILED;
	}
	catch (...){
		SendCriticalMessage(0, QObject::tr("Unknown exception"));

		m_lastCalibration.ResetData();
		return WS_FAILED;
	}

	m_lastCalibration.ResetData();
	return WS_FAILED;
}


} // namespace iocv


