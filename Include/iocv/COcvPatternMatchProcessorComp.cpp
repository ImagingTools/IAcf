#include <iocv/COcvPatternMatchProcessorComp.h>


 // ACF includes
#include <istd/CIndex2d.h>
#include <ibase/CSize.h>
#include <iprm/TParamsPtr.h>
#include <iimg/IBitmap.h>
#include <iprm/IEnableableParam.h>
#include <imeas/INumericValue.h>
#include <iimg/CScanlineMask.h>
#include <i2d/IObject2d.h>

// OpenCV includes
#include <opencv2/imgproc/imgproc.hpp>

namespace {
	cv::Mat FromBitmap(const iimg::IBitmap* inputBitmapPtr, const i2d::IObject2d* aoiPtr = nullptr){
		const istd::CIndex2d imageSize = inputBitmapPtr->GetImageSize();
		const int lineSizeBytes = inputBitmapPtr->GetLineBytesCount();

		if (aoiPtr == nullptr){
			cv::Mat matrix(imageSize.GetY(), imageSize.GetX(), CV_8U, lineSizeBytes);
			for (int y = 0; y < imageSize.GetY(); ++y) {
				const quint8* inputBitmapLinePtr = (const quint8*)inputBitmapPtr->GetLinePtr(y);
				quint8* outputLinePtr = matrix.ptr<quint8>(y);
				std::memcpy(outputLinePtr, inputBitmapLinePtr, lineSizeBytes);
			}

			return matrix;
		}
		else{
			i2d::CRectangle bBox = aoiPtr->GetBoundingBox();
			i2d::CRect imageRect = i2d::CRect(inputBitmapPtr->GetBoundingBox());
			i2d::CRect aoiRect = i2d::CRect(aoiPtr->GetBoundingBox());
			aoiRect.GetIntersection(imageRect);
			const int pixelBits = inputBitmapPtr->GetPixelBitsCount();
			const int bytesPerPixel = (int)(pixelBits / 8);
			const unsigned int shift = bytesPerPixel * bBox.GetLeft();

			qint8* startPtr = (qint8*)inputBitmapPtr->GetLinePtr(bBox.GetTop()) + shift;
			cv::Mat matrix = cv::Mat(bBox.GetHeight(), bBox.GetWidth(), CV_8UC1, startPtr, inputBitmapPtr->GetLinesDifference());
			return matrix;
		}
	}
}


namespace iocv
{

// reimplemented (iproc::IProcessor)

iproc::IProcessor::TaskState COcvPatternMatchProcessorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	imeas::INumericValue* outputResultPtr = dynamic_cast<imeas::INumericValue*>(outputPtr);
	if (outputResultPtr == nullptr){
		SendWarningMessage(0, "Output is not i2d::CVector2d", "COcvPatternMatchProcessorComp");

		return TS_INVALID;
	}

	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		SendWarningMessage(0, "Input bitmap is not set");

		return TS_INVALID;
	}

	if (inputBitmapPtr->IsEmpty()){
		SendWarningMessage(0, "Input bitmap is empty.");

		return TS_INVALID;
	}

	istd::CIndex2d inputImageSize = inputBitmapPtr->GetImageSize();
	//const int inputImageWidth = inputImageSize.GetX();
	//const int inputImageHeight = inputImageSize.GetY();

	cv::TemplateMatchModes mode;
	mode = (cv::TemplateMatchModes)*m_defaultMethodAttrPtr;
	if (m_methodParamIdAttrPtr.IsValid()){
		iprm::TParamsPtr<imeas::INumericValue> matchingMethodParamPtr(paramsPtr, *m_methodParamIdAttrPtr);
		if (matchingMethodParamPtr.IsValid()){
			mode = (cv::TemplateMatchModes)((int)matchingMethodParamPtr->GetValues()[0]);
		}
	}

	cv::Mat inputMatrix;

	i2d::CRectangle aoiBoundingBox(inputBitmapPtr->GetBoundingBox());

	if (m_aoiIdAttrPtr.IsValid()) {
		iprm::TParamsPtr<i2d::IObject2d> aoiParamPtr(paramsPtr, *m_aoiIdAttrPtr);
		if (aoiParamPtr.IsValid()) {
			istd::TUniqueInterfacePtr<i2d::IObject2d> transformedRegionPtr;
			transformedRegionPtr.MoveCastedPtr<istd::IChangeable>(aoiParamPtr->CloneMe());
			if (transformedRegionPtr.IsValid()) {
				const i2d::ICalibration2d* calibrationPtr = transformedRegionPtr->GetCalibration();
				if (calibrationPtr != NULL) {
					transformedRegionPtr->Transform(*calibrationPtr);

					transformedRegionPtr->SetCalibration(NULL);
				}

				aoiParamPtr.SetPtr(transformedRegionPtr.GetPtr());
				aoiBoundingBox = aoiParamPtr->GetBoundingBox();
			}
			inputMatrix = FromBitmap(inputBitmapPtr, aoiParamPtr.GetPtr());
			aoiBoundingBox = aoiParamPtr->GetBoundingBox();
		}
	}
	else{
		inputMatrix = FromBitmap(inputBitmapPtr);
	}

	int imageWidth = inputMatrix.cols;
	int imageHeight = inputMatrix.rows;

	cv::Mat templateMatrix;
	iprm::TParamsPtr<iimg::IBitmap> templateParamPtr(paramsPtr, *m_templateParamIdAttrPtr);
	if (templateParamPtr!= nullptr){
		templateMatrix = FromBitmap(templateParamPtr.GetPtr());
	}

	cv::Mat resultMatrix(imageHeight, imageWidth, CV_8U);

	if ((templateMatrix.cols >= inputMatrix.cols) || (templateMatrix.rows >= inputMatrix.rows)){
		SendErrorMessage(0, QString("Template exceeds image size"), "OcvPatternMatchProcessor");
		return TS_INVALID;
	}

	cv::matchTemplate(inputMatrix, templateMatrix, resultMatrix, mode);
	//cv::normalize(resultMatrix, resultMatrix, 0, 1, cv::NORM_MINMAX);
	
	double minVal; double maxVal;
	cv::Point minLoc; cv::Point maxLoc;
	cv::Point matchLoc(0,0);
	cv::minMaxLoc(resultMatrix, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
	if (mode > MM_SQDIFF_NORMED){
		if (maxVal > 0.986)
			matchLoc = maxLoc;
		else {
			SendInfoMessage(0, QString("Template matched with cost lower than 0.986"), "OcvPatternMatchProcessor");
		}
	}
	else{
		matchLoc = minLoc;
	}

	const double x = matchLoc.x + aoiBoundingBox.GetLeft();
	const double y = matchLoc.y + aoiBoundingBox.GetTop();
	imath::CVarVector result(2, 0.0);
	result.SetElement(0, x);
	result.SetElement(1, y);

	outputResultPtr->SetValues(result);

	return TS_OK;
}




} // namespace iocv


