#pragma once


// ACF-Solutions includes
#include <iproc/TSyncProcessorCompBase.h>
#include <iimg/IBitmap.h>
// OpenCV includes
#include <opencv2/imgproc/imgproc.hpp>

namespace iocv
{


/**
	Pattern Match processor.
*/
class COcvPatternMatchProcessorComp: public iproc::CSyncProcessorCompBase
{
public:
	typedef iproc::CSyncProcessorCompBase BaseClass;
	
	enum MatchingMethod {
		MM_SQDIFF = 0,
		MM_SQDIFF_NORMED,
		MM_CCORR,
		MM_CCORR_NORMED,
		MM_CCOEFF,
		MM_CCOEFF_NORMED
	};

	I_BEGIN_COMPONENT(COcvPatternMatchProcessorComp);
		I_ASSIGN(m_methodParamIdAttrPtr, "MethodParamId", "ID of the filter dimension parameter in the parameter set", false, "MethodParamId");
		I_ASSIGN(m_defaultMethodAttrPtr, "DefaultMatchingMethod", "Default matching method:0-Sq diff\n1-Sq diff normed\n\
			2-Cross correlation\n3-Cross correlation normed\n4-Correlation coefficient\n5-Correlation coefficient normed", true, 3);
		I_ASSIGN(m_templateParamIdAttrPtr, "TemplateParamId", "ID of the segmentation offset parameter in the parameter set", true, "TemplateParamId");
		I_ASSIGN(m_aoiIdAttrPtr, "AoiParamId", "ID of the AOI parameter in the parameter set", false, "AoiParamId");
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual iproc::IProcessor::TaskState DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL) override;

protected:


private:
	I_ATTR(QByteArray, m_methodParamIdAttrPtr);
	I_ATTR(int, m_defaultMethodAttrPtr);
	I_ATTR(QByteArray, m_templateParamIdAttrPtr);
	I_ATTR(QByteArray, m_aoiIdAttrPtr);
};


} // namespace iocv


