#pragma once


// ACF-Solutions includes
#include <iocv/COcvProcessorCompBase.h>


namespace iocv
{


/**
	Adaptive binarization processor.
*/
class COcvAdaptiveBinarizationComp: public iocv::COcvProcessorCompBase
{
public:
	typedef iocv::COcvProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(COcvAdaptiveBinarizationComp);
		I_ASSIGN(m_filterSizeParamsIdAttrPtr, "FilterSizeParamsId", "ID of the filter dimension parameter in the parameter set", true, "FilterSize");
		I_ASSIGN(m_segmentationOffsetIdAttrPtr, "SegmentationOffsetParamsId", "ID of the segmentation offset parameter in the parameter set", true, "SegmentationOffset");
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual iproc::IProcessor::TaskState DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL) override;

private:
	I_ATTR(QByteArray, m_filterSizeParamsIdAttrPtr);
	I_ATTR(QByteArray, m_segmentationOffsetIdAttrPtr);
};


} // namespace iocv


