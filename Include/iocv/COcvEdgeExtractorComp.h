#pragma once


// ACF includes
#include <iproc/TSyncProcessorCompBase.h>
#include <iedge/IEdgesExtractor.h>


namespace iocv
{


class COcvEdgeExtractorComp: public iproc::TSyncProcessorCompBase<iedge::IEdgesExtractor>
{
public:
	typedef iproc::TSyncProcessorCompBase<iedge::IEdgesExtractor> BaseClass;

	I_BEGIN_COMPONENT(COcvEdgeExtractorComp);
		I_ASSIGN(m_aoiParamIdAttrPtr, "AoiParamId", "ID of area of interest in parameter set", false, "AoiParams");
		I_ASSIGN(m_defaultAoiCompPtr, "DefaultAoi", "Area of interest used if not specified in parameters", false, "DefaultAoi");
	I_END_COMPONENT;

public:
	//	reimplemented (iedge::IEdgesExtractor)
	virtual bool DoContourExtraction(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap& bitmap,
				iedge::CEdgeLineContainer& result) const;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);

private:
	I_ATTR(QByteArray, m_aoiParamIdAttrPtr);
	I_REF(i2d::IObject2d, m_defaultAoiCompPtr);
};


} // namespace iocv


