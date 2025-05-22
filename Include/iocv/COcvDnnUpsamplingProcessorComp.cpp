#include <iocv/COcvDnnUpsamplingProcessorComp.h>


 // ACF includes
#include <istd/CIndex2d.h>
#include <ibase/CSize.h>
#include <iprm/TParamsPtr.h>
#include <iimg/IBitmap.h>
#include <i2d/ICalibration2d.h>
#include <i2d/CAffine2d.h>

// OpenCV includes
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn_superres.hpp>


namespace iocv
{


// protected methods

// reimplemented (iproc::IProcessor)

iproc::IProcessor::TaskState COcvDnnUpsamplingProcessorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	m_perspectiveCalibration = icalib::CPerspectiveCalibration2d();

	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		SendWarningMessage(0, "Input bitmap is not set");

		return TS_INVALID;
	}

	if (inputBitmapPtr->IsEmpty()){
		SendWarningMessage(0, "Input bitmap is empty");

		return TS_INVALID;
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (outputBitmapPtr == NULL){
		SendWarningMessage(0, "Output bitmap is not set");

		return TS_INVALID;
	}

	iprm::TParamsPtr<imeas::INumericValue> scaleParamPtr(paramsPtr, m_scaleParamIdAttrPtr, m_defaultScaleCompPtr);
	if (!scaleParamPtr.IsValid()){
		SendErrorMessage(0, "No target scale was set");

		return TS_INVALID;
	}

	imath::CVarVector scales = scaleParamPtr->GetValues();
	int scalesCount = scales.GetElementsCount();
	if (scalesCount < 1){
		SendErrorMessage(0, "Processing scale can't have dimension smaller 1");

		return TS_INVALID;
	}

	istd::CIndex2d inputBitmapSize = inputBitmapPtr->GetImageSize();
	int inputImageWidth = inputBitmapSize.GetX();
	int inputImageHeight = inputBitmapSize.GetY();

	double scaleX = scales[0];
	double scaleY = scaleX;
	if (scalesCount > 1){
		scaleY = scales[1]; 
	}

	int outputImageWidth = inputImageWidth * scaleX;
	int outputImageHeight = inputImageHeight * scaleY;

	// 8-byte alignment needed for OpenCV implementation:
	int allginedOutputImageWidth = ((outputImageWidth << 3) + 63) >> 3;

	double alignFactor = allginedOutputImageWidth / double(outputImageWidth);

	outputImageWidth = allginedOutputImageWidth;
	outputImageHeight = int(outputImageHeight * alignFactor + 0.5);

	cv::Mat inputMatrix(inputImageHeight, inputImageWidth, CV_8UC1, (qint8*)inputBitmapPtr->GetLinePtr(0));
	cv::Mat outputMatrix;// (outputImageHeight, outputImageWidth, CV_8UC1, (qint8*)outputBitmapPtr->GetLinePtr(0));

	cv::_InputArray input(inputMatrix);
	cv::_OutputArray output(outputMatrix);

	std::string modelPath = "C:/Work/Branches/RtVision.3d/V4.4.0/ExtLibs/Include/opencv-4.8/opencv2/models/FSRCNN_x2.pb";
	cv::dnn_superres::DnnSuperResImpl srProcessor;
	std::string algorithm = "fsrcnn";
	srProcessor.readModel(modelPath);
	srProcessor.setModel(algorithm, 2);
	srProcessor.upsample(inputMatrix, outputMatrix);

	const int sizeX = outputMatrix.cols;
	const int sizeY = outputMatrix.rows;
	int pixelBits = inputBitmapPtr->GetPixelBitsCount();
	int pixelBytes = pixelBits / 8;

	if (!outputBitmapPtr->CreateBitmap(inputBitmapPtr->GetPixelFormat(), istd::CIndex2d(sizeX, sizeY), pixelBits)){
		SendCriticalMessage(0, "Could not create output bitmap");
		return TS_INVALID;
	}
	
	const int rangeStart = 0;
	const int rangeEnd = sizeY;
	const int bytesToCopy = (rangeEnd - rangeStart) * pixelBytes;

	for (int y = 0; y < sizeY; y++){
		quint8* destinationLinePtr = (quint8*)outputBitmapPtr->GetLinePtr(y);
		cv::Mat sourceRow = outputMatrix.row(y);
		std::memcpy(destinationLinePtr, sourceRow.col(0).data, bytesToCopy);

	}


	const i2d::ICalibration2d* calibrationPtr = inputBitmapPtr->GetCalibration();
	if (calibrationPtr != nullptr){
		i2d::CAffine2d transform;
		calibrationPtr->GetLocalTransform(i2d::CVector2d(0, 0), transform);
		transform.SetDeformMatrix(transform.GetDeformMatrix() * 2);
		m_perspectiveCalibration.CopyFrom(*calibrationPtr);
		m_perspectiveCalibration.SetAffinePart(transform);
		outputBitmapPtr->SetCalibration(&m_perspectiveCalibration);
	}

	return TS_OK;
}


} // namespace iocv


