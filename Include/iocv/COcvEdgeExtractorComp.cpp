#include <iocv/COcvEdgeExtractorComp.h>


// OpenCV includes
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/opencv.hpp>

// ACF includes
#include <istd/CChangeGroup.h>
#include <iimg/CBitmapBase.h>
#include <iimg/CScanlineMask.h>
#include <iprm/TParamsPtr.h>
#include <i2d/CPolygon.h>
#include <ilog/CExtMessage.h>
#include <iimg/CBitmap.h>
#include <iedge/CEdgeLineContainer.h>


namespace iocv
{


// public methods

// reimplemented (iedge::IEdgesExtractor)

bool COcvEdgeExtractorComp::DoContourExtraction(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap& bitmap,
			iedge::CEdgeLineContainer& result) const
{
	if (bitmap.GetPixelFormat() != iimg::IBitmap::PF_GRAY){
		SendErrorMessage(0, "Invalid input image format. Expected is a 8-bit-grayscale image");

		return false;
	}

	iedge::CEdgeLineContainer* edgesContainerPtr = dynamic_cast<iedge::CEdgeLineContainer*>(&result);
	if (edgesContainerPtr == NULL){
		SendErrorMessage(0, "Edge container not defined");

		return false;
	}

	iprm::TParamsPtr<i2d::IObject2d> aoiPtr(paramsPtr, m_aoiParamIdAttrPtr, m_defaultAoiCompPtr);

	istd::CChangeGroup resultNotifier(edgesContainerPtr);

	edgesContainerPtr->Reset();

	ibase::CSize size = bitmap.GetImageSize();

	// Initialize input bitmap:
	cv::Mat tmpBinaryImage;

	// Filter out masked points:
	if (aoiPtr.IsValid()){
		iimg::CScanlineMask invertedMask;
		invertedMask.SetCalibration(bitmap.GetCalibration());

		i2d::CRect clipArea(size);
		if (!invertedMask.CreateFromGeometry(*aoiPtr, &clipArea)) {
			SendErrorMessage(0, QObject::tr("AOI type is not supported"));

			return false;
		}

		iimg::CScanlineMask mask;
		invertedMask.GetInverted(clipArea, mask);

		iimg::CBitmap maskedBitmap;
		maskedBitmap.CopyFrom(bitmap);

		istd::CIntRange lineRange(0, size.GetX());

		for (int y = 0; y < size.GetY(); ++y){
			const istd::CIntRanges* outputRangesPtr = mask.GetPixelRanges(y);
			if (outputRangesPtr != NULL){
				quint8* inputLinePtr = static_cast<quint8*>(maskedBitmap.GetLinePtr(y));

				istd::CIntRanges::RangeList rangeList;
				outputRangesPtr->GetAsList(lineRange, rangeList);
				for (		istd::CIntRanges::RangeList::ConstIterator iter = rangeList.constBegin();
							iter != rangeList.constEnd();
							++iter){
					const istd::CIntRange& rangeH = *iter;
					Q_ASSERT(rangeH.GetMinValue() >= 0);
					Q_ASSERT(rangeH.GetMaxValue() <= size.GetX());

					for (int x = rangeH.GetMinValue(); x < rangeH.GetMaxValue(); ++x){
						inputLinePtr[x] = 0;
					}
				}
			}
		}

		void* imageDataBufferPtr = const_cast<void*>(maskedBitmap.GetLinePtr(0));
		cv::Mat inputBitmap(size.GetY(), size.GetX(), CV_8UC1, imageDataBufferPtr, maskedBitmap.GetLinesDifference());
		tmpBinaryImage = inputBitmap.clone();
	}
	else{
		void* imageDataBufferPtr = const_cast<void*>(bitmap.GetLinePtr(0));
		cv::Mat inputBitmap(size.GetY(), size.GetX(), CV_8UC1, imageDataBufferPtr, bitmap.GetLinesDifference());

		tmpBinaryImage = inputBitmap.clone();
	}

	int aproximationMode = CV_CHAIN_APPROX_NONE;

	iprm::TParamsPtr<iprm::ISelectionParam> approximationModeParamPtr(paramsPtr, m_approxModeParamIdAttrPtr, m_defaultApproxModeCompPtr);
	if (approximationModeParamPtr.IsValid()){
		int selectedMode = approximationModeParamPtr->GetSelectedOptionIndex();
		if (selectedMode >= 0){
			aproximationMode = m_approxModes.GetOpenCvMode(selectedMode);
		}
	}

	double reduceFactor = 0.0;
	iprm::TParamsPtr<imeas::INumericValue> reduceFactorParamPtr(paramsPtr, m_reduceFactorParamIdAttrPtr, m_defaultReduceFactorCompPtr);
	if (reduceFactorParamPtr.IsValid()){
		reduceFactor = reduceFactorParamPtr->GetValues()[0];
	}

	int contourMode = CV_RETR_LIST;
	iprm::TParamsPtr<iprm::ISelectionParam> contourModeParamPtr(paramsPtr, m_contourModeParamIdAttrPtr, m_defaultContourModeCompPtr);
	if (contourModeParamPtr.IsValid()){
		int selectedMode = contourModeParamPtr->GetSelectedOptionIndex();
		if (selectedMode >= 0){
			contourMode = m_contourModes.GetOpenCvMode(selectedMode);
		}
	}

	// Get contours from the binary image:
	std::vector<std::vector<cv::Point> > contours;
	findContours(tmpBinaryImage, contours, contourMode, aproximationMode);

	for (int contourIndex = 0; contourIndex < int(contours.size()); ++contourIndex){
		iedge::CEdgeLine& resultLine = edgesContainerPtr->PushBack(iedge::CEdgeLine());

		const std::vector<cv::Point>& contour = contours[contourIndex];
		std::vector<cv::Point> approxContour;

		double arcLength = cv::arcLength(cv::Mat(contour), true);

		cv::approxPolyDP(contour, approxContour,  arcLength * reduceFactor, true);

		resultLine.SetNodesCount(int(approxContour.size()));

		for (int nodeIndex = 0; nodeIndex < int(approxContour.size()); ++nodeIndex){
			iedge::CEdgeNode& nodeRef = resultLine.GetNodeRef(nodeIndex);

			nodeRef.SetPosition(i2d::CVector2d(approxContour[nodeIndex].x + 0.5, approxContour[nodeIndex].y + 0.5));
			nodeRef.SetWeight(0.5);
		}

		resultLine.SetClosed(true);
	}
		
	return true;
}


// reimplemented (iproc::IProcessor)

int COcvEdgeExtractorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* result,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		SendWarningMessage(0, "Input bitmap is not set");

		return TS_INVALID;
	}

	if (inputBitmapPtr->IsEmpty()){
		SendWarningMessage(0, "Input bitmap is empty.");

		return TS_INVALID;
	}

	iedge::CEdgeLineContainer* edgesContainerPtr = dynamic_cast<iedge::CEdgeLineContainer*>(result);
	if (edgesContainerPtr == NULL){
		return TS_INVALID;
	}

	return DoContourExtraction(paramsPtr, *inputBitmapPtr, *edgesContainerPtr) ? TS_OK : TS_INVALID;
}


// public methods of the embedded class OpenCvOptionsList

int COcvEdgeExtractorComp::OpenCvOptionsList::GetOpenCvMode(int index) const
{
	return m_options[index].cvMode;
}


// reimplemented (iprm::IOptionsList)

int COcvEdgeExtractorComp::OpenCvOptionsList::GetOptionsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID;
}


int COcvEdgeExtractorComp::OpenCvOptionsList::GetOptionsCount() const
{
	return m_options.count();
}


QString COcvEdgeExtractorComp::OpenCvOptionsList::GetOptionName(int index) const
{
	return m_options[index].name;
}


QString COcvEdgeExtractorComp::OpenCvOptionsList::GetOptionDescription(int index) const
{
	return m_options[index].description;
}


QByteArray COcvEdgeExtractorComp::OpenCvOptionsList::GetOptionId(int index) const
{
	return m_options[index].name.toUtf8();
}


bool COcvEdgeExtractorComp::OpenCvOptionsList::IsOptionEnabled(int /*index*/) const
{
	return true;
}


// public methods of the embedded class ApproxModeList

COcvEdgeExtractorComp::ApproxModeList::ApproxModeList()
{
	m_options.push_back(OpenCvOption(CV_CHAIN_APPROX_NONE, "None", "No contour approximation used"));
	m_options.push_back(OpenCvOption(CV_CHAIN_APPROX_SIMPLE, "Simple", "Reduce contour to vertical, horizontal and diagonal line segments"));
	m_options.push_back(OpenCvOption(CV_CHAIN_APPROX_TC89_L1, "Teh-Chin-L1", "Teh-Chin chain approximation algorithm"));
	m_options.push_back(OpenCvOption(CV_CHAIN_APPROX_TC89_KCOS, "Teh-Chin-KCOS", "Teh-Chin chain approximation algorithm"));
}


// public methods of the embedded class ContourModeList

COcvEdgeExtractorComp::ContourModeList::ContourModeList()
{
	m_options.push_back(OpenCvOption(CV_RETR_LIST, "No Hierarchy", "Retrieve all contours without creation of the parent-child relationship"));
	m_options.push_back(OpenCvOption(CV_RETR_EXTERNAL, "Outer-Contours", "Retrieve extreme outer-contours only"));
	m_options.push_back(OpenCvOption(CV_RETR_CCOMP, "Two-Level-Hierarchy", "Retrieve contours arranged in two-level-hierarchy (parent-child)"));
	m_options.push_back(OpenCvOption(CV_RETR_TREE, "Full Hierarchy", "Retrieve contours arranged as a tree"));
}



} // namespace iocv


