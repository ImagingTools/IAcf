#pragma once


// ACF-Solutions includes
#include <iocv/COcvProcessorCompBase.h>


namespace iocv
{


/**
	Universal OpenCV-based binarization processor.
*/
class COcvBinarizationProcessorComp : public iocv::COcvProcessorCompBase
{
public:
	typedef iocv::COcvProcessorCompBase BaseClass;

	enum OperationMode {
		BIN_ADAPTIVE = 0,
		BIN_THRESHOLD = 1,
		BIN_OTSU = 2,
		BIN_TRIANGLE = 3,
		_BIN_LAST
	};
	
	I_BEGIN_COMPONENT(COcvBinarizationProcessorComp);
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual iproc::IProcessor::TaskState DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL) override;
};


} // namespace iocv


