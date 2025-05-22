#include <iocv/CCharucoCalibrationSupplierComp.h>

#include <iostream>

// opencv includes
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core.hpp>


// ACF include
#include <ilog/CExtMessage.h>
#include <iprm/TParamsPtr.h>
#include <i2d/CPosition2d.h>

// ACF-Solutions includes
#include <iipr/CPerspCalibFinder.h>
#include <iipr/CPointGridFeature.h>


namespace {
	static bool saveCameraParams(const std::string& filename, cv::Size imageSize, float aspectRatio, int flags,
		const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs, double totalAvgErr) {
		cv::FileStorage fs(filename, cv::FileStorage::WRITE);
		if (!fs.isOpened())
			return false;

		time_t tt;
		time(&tt);
		struct tm* t2 = localtime(&tt);
		char buf[1024];
		strftime(buf, sizeof(buf) - 1, "%c", t2);

		fs << "calibration_time" << buf;
		fs << "image_width" << imageSize.width;
		fs << "image_height" << imageSize.height;

		if (flags & cv::CALIB_FIX_ASPECT_RATIO) fs << "aspectRatio" << aspectRatio;

		if (flags != 0) {
			sprintf(buf, "flags: %s%s%s%s",
				flags & cv::CALIB_USE_INTRINSIC_GUESS ? "+use_intrinsic_guess" : "",
				flags & cv::CALIB_FIX_ASPECT_RATIO ? "+fix_aspectRatio" : "",
				flags & cv::CALIB_FIX_PRINCIPAL_POINT ? "+fix_principal_point" : "",
				flags & cv::CALIB_ZERO_TANGENT_DIST ? "+zero_tangent_dist" : "");
		}
		fs << "flags" << flags;
		fs << "camera_matrix" << cameraMatrix;
		fs << "distortion_coefficients" << distCoeffs;
		fs << "avg_reprojection_error" << totalAvgErr;
		return true;
	}
}


namespace iocv
{



struct PointGridConsumer : public iipr::IFeaturesConsumer
{
	// reimplemented (iipr::IFeaturesConsumer)

	virtual void ResetFeatures() override
	{
		m_points.Reset();
	}

	virtual bool AddFeature(const imeas::INumericValue* featurePtr, bool* /*isFullPtr = NULL*/) override
	{
		if (featurePtr != NULL) {
			const icalib::CPointGrid* pointGridPtr = dynamic_cast<const icalib::CPointGrid*>(featurePtr);
			if (pointGridPtr != NULL) {
				m_points.CopyFrom(*pointGridPtr);
			}

			delete featurePtr;
		}

		return true;
	}

	icalib::CPointGrid m_points;
};


// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CCharucoCalibrationSupplierComp::GetCalibration() const
{
	return GetWorkProduct();
}


//protected methods

bool CCharucoCalibrationSupplierComp::CalculateCalibration(const iimg::IBitmap& inputImage, icalib::CPerspectiveCalibration2d& result) const
{
	result.Reset();
	if (inputImage.IsEmpty()){
		return false;
	}

	cv::Mat inputMatrix = cv::Mat(inputImage.GetImageSize().GetY(), inputImage.GetImageSize().GetX(), CV_8UC1, (quint8*)inputImage.GetLinePtr(0));

	int squaresX = 10;
	int squaresY = 10;
	float squareLength = 0.014;
	float markerLength = 0.0115;

	float aspectRatio = 1;
	bool refindStrategy = false;
	int calibrationFlags = 0;

	calibrationFlags |= cv::CALIB_FIX_PRINCIPAL_POINT;

	cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();

	/*DICT_4x4_250 = 2*/
	cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PredefinedDictionaryType(2));

	// Create charuco board object
	cv::aruco::CharucoBoard board(cv::Size(squaresX, squaresY), squareLength, markerLength, dictionary);
	cv::aruco::CharucoParameters charucoParams;

	if (refindStrategy) {
		charucoParams.tryRefineMarkers = true;
	}

	cv::aruco::CharucoDetector detector(board, charucoParams, detectorParams);

	// Collect data from each frame
	std::vector<cv::Mat> allCharucoCorners;
	std::vector<cv::Mat> allCharucoIds;

	std::vector<std::vector<cv::Point2f>> allImagePoints;
	std::vector<std::vector<cv::Point3f>> allObjectPoints;

	std::vector<cv::Mat> allImages;
	cv::Size imageSize;

	cv::Mat image, imageCopy;
	image = inputMatrix;
	// get image

	for (int i = 0; i < 4; ++i) {
		std::vector<int> markerIds;
		std::vector<std::vector<cv::Point2f>> markerCorners, rejectedMarkers;
		cv::Mat currentCharucoCorners;
		cv::Mat currentCharucoIds;
		std::vector<cv::Point3f> currentObjectPoints;
		std::vector<cv::Point2f> currentImagePoints;

		// Detect ChArUco board
		detector.detectBoard(image, currentCharucoCorners, currentCharucoIds);

		// Draw results
		image.copyTo(imageCopy);
		if (!currentCharucoIds.empty()) {
			cv::aruco::drawDetectedMarkers(imageCopy, markerCorners);
		}

		if (currentCharucoCorners.total() > 3) {
			cv::aruco::drawDetectedCornersCharuco(imageCopy, currentCharucoCorners, currentCharucoIds);
		}

		cv::imshow("out", imageCopy);

		// Wait for key pressed

		if (currentCharucoCorners.total() > 3) {
			// Match image points
			board.matchImagePoints(currentCharucoCorners, currentCharucoIds, currentObjectPoints, currentImagePoints);

			if (currentImagePoints.empty() || currentObjectPoints.empty()) {
				std::cout << "Point matching failed, try again." << std::endl;
				return false;
			}

			std::cout << "Frame captured" << std::endl;

			allCharucoCorners.push_back(currentCharucoCorners);
			allCharucoIds.push_back(currentCharucoIds);
			allImagePoints.push_back(currentImagePoints);
			allObjectPoints.push_back(currentObjectPoints);
			allImages.push_back(image);

			imageSize = image.size();
		}
	}

	if (allCharucoCorners.size() < 4) {
		std::cerr << "Not enough corners for calibration" << std::endl;
		return false;
	}

	cv::Mat cameraMatrix, distCoeffs;

	if (calibrationFlags & cv::CALIB_FIX_ASPECT_RATIO) {
		cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
		cameraMatrix.at<double>(0, 0) = aspectRatio;
	}

	// Calibrate camera using ChArUco
	double repError = calibrateCamera(
		allObjectPoints, allImagePoints, imageSize,
		cameraMatrix, distCoeffs, cv::noArray(), cv::noArray(), cv::noArray(),
		cv::noArray(), cv::noArray(), calibrationFlags
	);

	std::string outputFile = "C:\\Work\\Images\\my test\\ChAruco\\CameraCalibration.txt";
	bool saveOk = saveCameraParams(
				outputFile,
				imageSize,
				aspectRatio,
				calibrationFlags,
				cameraMatrix,
				distCoeffs,
				repError);

	Q_UNUSED(saveOk);

	cv::Mat newCameraMatrix = cv::getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 0);

	i2d::CMatrix2d rotationMatrix(
		newCameraMatrix.at<double>(0, 0), newCameraMatrix.at<double>(1, 0),
		newCameraMatrix.at<double>(0, 1), newCameraMatrix.at<double>(1, 1));

	i2d::CVector2d translation(newCameraMatrix.at<double>(0, 2), newCameraMatrix.at<double>(1, 2));
	i2d::CVector2d perspective(newCameraMatrix.at<double>(2, 0), newCameraMatrix.at<double>(2, 1));


	result.SetAffinePart(i2d::CAffine2d(rotationMatrix, translation));
	result.SetPerspAxis(perspective);
	
#if 0
	// Show interpolated charuco corners for debugging
	if (showChessboardCorners) {
		for (size_t frame = 0; frame < allImages.size(); frame++) {
			cv::Mat imageCopy = allImages[frame].clone();

			if (allCharucoCorners[frame].total() > 0) {
				cv::aruco::drawDetectedCornersCharuco(
					imageCopy, allCharucoCorners[frame], allCharucoIds[frame]
				);
			}

			cv::imshow("out", imageCopy);
			char key = (char)cv::waitKey(0);
			if (key == 27) {
				break;
			}
		}
	}
#endif // DEBUG OUTPUT

	return true;
}


// reimplemented (iinsp::TSupplierCompWrap)

iinsp::ISupplier::WorkStatus CCharucoCalibrationSupplierComp::ProduceObject(ProductType& result) const
{
	if (!m_bitmapProviderCompPtr.IsValid()){
		return WS_FAILED;
	}

	const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
	if (bitmapPtr == NULL){
		AddMessage(new ilog::CMessage(ilog::CMessage::IC_ERROR, 0, QT_TR_NOOP("Empty input image"), "PerspectiveCalibrationSupplier", "iocv::CCharucoCalibrationSupplierComp"));

		return WS_FAILED;
	}

	Timer performanceTimer(this, "Perspective calibration");

	if (!CalculateCalibration(*bitmapPtr, result)){
		return WS_FAILED;
	}

	return WS_OK;
}


} // namespace iocv


