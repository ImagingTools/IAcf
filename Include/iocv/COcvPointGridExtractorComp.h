// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-IAcf-Commercial
#pragma once


// ACF-Solutions includes
#include <iproc/TSyncProcessorCompBase.h>
#include <iipr/IImageToFeatureProcessor.h>


#include <opencv2/core/core.hpp>


namespace iocv
{


class COcvPointGridExtractorComp: public iproc::TSyncProcessorCompBase<iipr::IImageToFeatureProcessor>
{
public:
	typedef iproc::TSyncProcessorCompBase<iipr::IImageToFeatureProcessor> BaseClass;

	I_BEGIN_COMPONENT(COcvPointGridExtractorComp);
		I_ASSIGN(m_resultConsumerCompPtr, "ResultConsumer", "Consumer of result messages with geometrical layout", false, "ResultConsumer");
		I_ASSIGN(m_defaultGridSizeCompPtr, "GridSize", "Component that provides the default grid size. Two numbers for horizontal (first) and verical (second) directions", false, "GridSize");
		I_ASSIGN(m_defaultPatternTypeAttrPtr, "PatternTypeDefault", "Type of pattern which is used for calibration.\n0 : Chessboard\n1: Circles grid\n2: ChArUco board", false, 0);
		I_ASSIGN(m_gridSizeParamIdAttrPtr, "GridSizeParamId", "Parameter-ID of the grid size values in the parameter set. Two numbers for horizontal (first) and verical (second) directions", false, "GridSize");
		I_ASSIGN(m_patternTypeParamIdAttrPtr, "PatternTypeParamId", "Parameter-ID of pattern type in the parameter set.\n0 : Chessboard\n1: Circles grid\n2:ChArUco board", false, "PatternType");
		I_ASSIGN(m_defaultRectangleCompPtr, "DefaultAOI", "Search area to loocking for pattern", false, "DefaultAOI");
		I_ASSIGN(m_aoiRecangleParamIdAttrPtr, "AOIParamId", "Parameter-ID of AOI Rectangle in ParamsSet (i2d::CRectangle)", false, "AOIParamId");
		I_ASSIGN(m_checkerBoardPointGridExtractor, "CheckerBoardExtractor", "Dedicated extractor of checkerboard", false, "CheckerBoardExtractor");
	I_END_COMPONENT;

	enum Pattern
	{
		PT_CHESSBOARD = 0,
		PT_CIRCLES_GRID
	};


	// reimplemented (iipr::IImageToFeatureProcessor)
	virtual iproc::IProcessor::TaskState DoExtractFeatures(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap& image,
				iipr::IFeaturesConsumer& results,
				ibase::IProgressManager* progressManagerPtr = NULL) override;

	// reimplemented (iproc::IProcessor)
	virtual iproc::IProcessor::TaskState DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL) override;

	Pattern GetPatternType() const;

private:
	void RetrievePatternType(const iprm::IParamsSet* paramsPtr);
	bool FindCharucoCorners(cv::Mat& view, std::vector<cv::Point2f>& pointBuf, std::vector<int>& ids) const;
	bool ChessboardCorners(
				cv::Mat& view,
				cv::Size& gridSize,
				std::vector<cv::Point2f>& pointBuf,
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap& image,
				iipr::IFeaturesConsumer& results) const;

	I_REF(ilog::IMessageConsumer, m_resultConsumerCompPtr);
	I_REF(imeas::INumericValue, m_defaultGridSizeCompPtr);
	I_ATTR(int, m_defaultPatternTypeAttrPtr);
	I_ATTR(QByteArray, m_gridSizeParamIdAttrPtr);
	I_ATTR(QByteArray, m_patternTypeParamIdAttrPtr);
	I_REF(i2d::CRectangle, m_defaultRectangleCompPtr);
	I_ATTR(QByteArray, m_aoiRecangleParamIdAttrPtr);
	I_REF(iipr::IImageToFeatureProcessor, m_checkerBoardPointGridExtractor);

	mutable bool resultsAdded;
	Pattern m_patternType;
};


} // namespace iocv


