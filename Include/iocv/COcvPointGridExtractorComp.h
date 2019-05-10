#pragma once


// ACF-Solutions includes
#include <iproc/TSyncProcessorCompBase.h>
#include <iipr/IImageToFeatureProcessor.h>


namespace iocv
{


class COcvPointGridExtractorComp: public iproc::TSyncProcessorCompBase<iipr::IImageToFeatureProcessor>
{
public:
	typedef iproc::TSyncProcessorCompBase<iipr::IImageToFeatureProcessor> BaseClass;

	I_BEGIN_COMPONENT(COcvPointGridExtractorComp);
		I_ASSIGN(m_resultConsumerCompPtr, "ResultConsumer", "Consumer of result messages with geometrical layout", false, "ResultConsumer");
		I_ASSIGN(m_defaultGridSizeCompPtr, "GridSize", "Component that provides the default grid size. Two numbers for horizontal (first) and verical (second) directions", false, "GridSize");
		I_ASSIGN(m_defaultPatternTypeAttrPtr, "PatternType", "Type of pattern which uses to look for points.\n0 : Chessboard\n1: Circles grid", false, 0);
		I_ASSIGN(m_gridSizeParamIdAttrPtr, "GridSizeParamId", "Parameter-ID of the grid size values in the parameter set. Two numbers for horizontal (first) and verical (second) directions", false, "GridSize");
		I_ASSIGN(m_patternTypeParamIdAttrPtr, "PatternTypeParamId", "Parameter-ID of pattern type in the parameter set.\n0 : Chessboard\n1: Circles grid", false, "PatternType");
		I_ASSIGN(m_defaultRectangleCompPtr, "DefaultAOI", "Search area to loocking for pattern", false, "DefaultAOI");
	I_END_COMPONENT;

	enum Pattern
	{
		PT_CHESSBOARD = 0,
		PT_CIRCLES_GRID
	};

	// reimplemented (iipr::IImageToFeatureProcessor)
	virtual int DoExtractFeatures(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap& image,
				iipr::IFeaturesConsumer& results,
				ibase::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);

private:
	I_REF(ilog::IMessageConsumer, m_resultConsumerCompPtr);
	I_REF(imeas::INumericValue, m_defaultGridSizeCompPtr);
	I_ATTR(int, m_defaultPatternTypeAttrPtr);
	I_ATTR(QByteArray, m_gridSizeParamIdAttrPtr);
	I_ATTR(QByteArray, m_patternTypeParamIdAttrPtr);
	I_REF(i2d::CRectangle, m_defaultRectangleCompPtr);
};


} // namespace iocv


