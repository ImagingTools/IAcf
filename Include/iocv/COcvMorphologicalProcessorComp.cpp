#include <iocv/COcvMorphologicalProcessorComp.h>


 // ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/CIndex2d.h>
#include <ibase/CSize.h>
#include <iprm/TParamsPtr.h>
#include <iimg/CGeneralBitmap.h>
#include <iimg/CScanlineMask.h>
#include <imeas/INumericValue.h>

// OpenCV includes
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>


namespace iocv
{


// protected methods

// reimplemented (CImageRegionProcessorCompBase)

bool COcvMorphologicalProcessorComp::ProcessImageRegion(
			const iimg::IBitmap& inputBitmap,
			const iprm::IParamsSet* paramsPtr,
			const i2d::IObject2d* aoiPtr,
			istd::IChangeable* outputPtr) const
{
	if (inputBitmap.IsEmpty()){
		SendWarningMessage(0, "Input bitmap is empty.");

		return false;
	}

	if (aoiPtr == NULL){
		SendWarningMessage(0, "Filter region is not defined");

		return false;
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (outputBitmapPtr == NULL){
		SendWarningMessage(0, "Output bitmap is not set");

		return false;
	}

	istd::CIndex2d inputBitmapSize = inputBitmap.GetImageSize();

	if (!outputBitmapPtr->CopyFrom(inputBitmap)){
		SendErrorMessage(0, "Data could not be copied from input bitmap to the output");

		return false;
	}

	iprm::TParamsPtr<imeas::INumericValue> filterSizePtr(paramsPtr, *m_filterSizeParamsIdAttrPtr);
	if (filterSizePtr == NULL){
		SendErrorMessage(0, "No fiter dimension was set");

		return false;
	}

	imath::CVarVector filterLengths = filterSizePtr->GetValues();
	int filterDimensionsCount = filterLengths.GetElementsCount();
	if (filterDimensionsCount < 1){
		SendErrorMessage(0, "Processing filter can't have dimension smaller 1");

		return false;
	}

	int kernelSize = filterLengths[0];

	int imageHeight = inputBitmapSize.GetY();
	int processingMode = GetProcessingMode(paramsPtr);
	int kernelType = GetKernelType(paramsPtr);

	cv::Mat kernelS = getStructuringElement(kernelType, cv::Size(kernelSize, kernelSize));

	int pixelFormat = inputBitmap.GetPixelFormat();
	switch (pixelFormat){
		case iimg::IBitmap::PF_GRAY:{
			cv::Mat inputMatrix(imageHeight, inputBitmap.GetLinesDifference(), CV_8UC1, (quint8*)inputBitmap.GetLinePtr(0));
			cv::Mat outputMatrix(imageHeight, outputBitmapPtr->GetLinesDifference(), CV_8UC1, (quint8*)outputBitmapPtr->GetLinePtr(0));
			cv::morphologyEx(inputMatrix, outputMatrix, processingMode, kernelS);
			break;
		}

		case iimg::IBitmap::PF_RGB:{
			cv::Mat inputMatrix(imageHeight, inputBitmap.GetLinesDifference(), CV_8UC3, (quint32*)inputBitmap.GetLinePtr(0));
			cv::Mat outputMatrix(imageHeight, outputBitmapPtr->GetLinesDifference(), CV_8UC3, (quint32*)outputBitmapPtr->GetLinePtr(0));
			cv::morphologyEx(inputMatrix, outputMatrix, processingMode, kernelS);
			break;
		}

		case iimg::IBitmap::PF_RGBA:{
			cv::Mat inputMatrix(imageHeight, inputBitmap.GetLinesDifference(), CV_8UC4, (quint32*)inputBitmap.GetLinePtr(0));
			cv::Mat outputMatrix(imageHeight, outputBitmapPtr->GetLinesDifference(), CV_8UC4, (quint32*)outputBitmapPtr->GetLinePtr(0));
			cv::morphologyEx(inputMatrix, outputMatrix, processingMode, kernelS);
			break;
		}

		case iimg::IBitmap::PF_GRAY16:{
			cv::Mat inputMatrix(imageHeight, inputBitmap.GetLinesDifference(), CV_16UC1, (quint16*)inputBitmap.GetLinePtr(0));
			cv::Mat outputMatrix(imageHeight, outputBitmapPtr->GetLinesDifference(), CV_16UC1, (quint16*)outputBitmapPtr->GetLinePtr(0));
			cv::morphologyEx(inputMatrix, outputMatrix, processingMode, kernelS);
			break;
		}

		case iimg::IBitmap::PF_GRAY32:{
			cv::Mat inputMatrix(imageHeight, inputBitmap.GetLinesDifference(), CV_32SC1, (quint32*)inputBitmap.GetLinePtr(0));
			cv::Mat outputMatrix(imageHeight, outputBitmapPtr->GetLinesDifference(), CV_32SC1, (quint32*)outputBitmapPtr->GetLinePtr(0));
			cv::morphologyEx(inputMatrix, outputMatrix, processingMode, kernelS);
			break;
		}

	default:
		return false;
	}

	return true;
}


// reimplemented (icomp::CComponentBase)

void COcvMorphologicalProcessorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	int modeIndex = 0;
	if (!m_erosionEnableAttrPtr.IsValid() || *m_erosionEnableAttrPtr){
		m_processingModes.InsertOption(QObject::tr("Erosion"), "Erosion");
		m_processingModeIndexMap[modeIndex++] = cv::MORPH_ERODE;
	}

	if (!m_dilatationEnableAttrPtr.IsValid() || *m_dilatationEnableAttrPtr){
		m_processingModes.InsertOption(QObject::tr("Dilation"), "Dilation");
		m_processingModeIndexMap[modeIndex++] = cv::MORPH_DILATE;
	}

	if (!m_openingEnableAttrPtr.IsValid() || *m_openingEnableAttrPtr){
		m_processingModes.InsertOption(QObject::tr("Opening"), "Opening");
		m_processingModeIndexMap[modeIndex++] = cv::MORPH_OPEN;
	}

	if (!m_closingEnableAttrPtr.IsValid() || *m_closingEnableAttrPtr){
		m_processingModes.InsertOption(QObject::tr("Closing"), "Closing");
		m_processingModeIndexMap[modeIndex++] = cv::MORPH_CLOSE;
	}

	if (!m_whiteTopEnableAttrPtr.IsValid() || *m_whiteTopEnableAttrPtr){
		m_processingModes.InsertOption(QObject::tr("White Top-Hat"), "WhiteTopHat");
		m_processingModeIndexMap[modeIndex++] = cv::MORPH_TOPHAT;
	}

	if (!m_blackTopEnableAttrPtr.IsValid() || *m_blackTopEnableAttrPtr){
		m_processingModes.InsertOption(QObject::tr("Black Top-Hat"), "BlackTopHat");
		m_processingModeIndexMap[modeIndex++] = cv::MORPH_BLACKHAT;
	}

	if (!m_morthoGradientEnableAttrPtr.IsValid() || *m_morthoGradientEnableAttrPtr){
		m_processingModes.InsertOption(QObject::tr("Morphological Gradient"), "MorphologicalGradient");
		m_processingModeIndexMap[modeIndex] = cv::MORPH_GRADIENT;
	}


	int formIndex = 0;
	if (!m_rectangleFormEnableAttrPtr.IsValid() || *m_rectangleFormEnableAttrPtr){
		m_filterForms.InsertOption(QObject::tr("Rectangle"), "Rectangle");
		m_filterFormIndexMap[formIndex] = cv::MORPH_RECT;
		formIndex++;
	}

	if (!m_circleFormEnableAttrPtr.IsValid() || *m_circleFormEnableAttrPtr){
		m_filterForms.InsertOption(QObject::tr("Circle"), "Circle");
		m_filterFormIndexMap[formIndex] = cv::MORPH_ELLIPSE;
		formIndex++;
	}

	if (!m_crossFormEnableAttrPtr.IsValid() || *m_crossFormEnableAttrPtr){
		m_filterForms.InsertOption(QObject::tr("Cross"), "Cross");
		m_filterFormIndexMap[formIndex] = cv::MORPH_CROSS;
		formIndex++;
	}
}


// private methods

int COcvMorphologicalProcessorComp::GetProcessingMode(const iprm::IParamsSet* paramsPtr) const
{
	int mode = *m_defaultProcessingModeAttrPtr;

	if ((paramsPtr != NULL) && m_processingModeIdAttrPtr.IsValid()){
		iprm::TParamsPtr<iprm::ISelectionParam> processingModeParamPtr(paramsPtr, *m_processingModeIdAttrPtr, false);
		if (processingModeParamPtr.IsValid()){
			int ind = processingModeParamPtr->GetSelectedOptionIndex();
			mode = m_processingModeIndexMap.contains(ind) ? 
				m_processingModeIndexMap[ind] : 0;
		}
	}

	if (mode < 0 || mode > cv::MORPH_BLACKHAT){
		mode = cv::MORPH_ERODE;
	}

	return mode;
}


int COcvMorphologicalProcessorComp::GetKernelType(const iprm::IParamsSet* paramsPtr) const
{
	int formType = *m_defaultFilterFormTypeAttrPtr;

	if ((paramsPtr != NULL) && m_filterFormTypeIdAttrPtr.IsValid()){
		iprm::TParamsPtr<iprm::ISelectionParam> filterFormTypeParamPtr(paramsPtr, *m_filterFormTypeIdAttrPtr, false);
		if (filterFormTypeParamPtr.IsValid()){
			int ind = filterFormTypeParamPtr->GetSelectedOptionIndex();
			formType = m_filterFormIndexMap.contains(ind) ? m_filterFormIndexMap[ind] : cv::MORPH_RECT;
		}
	}

	if (formType < cv::MORPH_RECT || formType > cv::MORPH_ELLIPSE){
		formType = cv::MORPH_RECT;
	}

	return formType;
}


} // namespace iocv


