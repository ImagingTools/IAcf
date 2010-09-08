#ifndef iipr_CImageProcessorCompBase_included
#define iipr_CImageProcessorCompBase_included


// ACF includes
#include "isys/ITimer.h"
#include "istd/TSmartPtr.h"
#include "iproc/TSyncProcessorCompBase.h"
#include "iimg/IBitmap.h"


namespace iipr
{


/**
	Basic implementation for a simple bitmap processor.
*/
class CImageProcessorCompBase: public iproc::CSyncProcessorCompBase
{
public:
	typedef iproc::CSyncProcessorCompBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CImageProcessorCompBase)
		I_ASSIGN(m_preprocessorCompPtr, "Preprocessor", "Pre-processing operator", false, "Preprocessor");
	I_END_COMPONENT

	CImageProcessorCompBase();

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr);

protected:
	// abstract methods
	virtual bool ProcessImage(
				const iprm::IParamsSet* paramsPtr, 
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage) = 0;

private:
	istd::TSmartPtr<isys::ITimer> m_timerPtr;

	I_REF(iproc::IProcessor, m_preprocessorCompPtr);
};


} // namespace iipr


#endif // !iipr_CImageProcessorCompBase_included

