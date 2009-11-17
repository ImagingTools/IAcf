#ifndef iipr_TImageProcessorCompBase_included
#define iipr_TImageProcessorCompBase_included


// ACF includes
#include "isys/ITimer.h"
#include "istd/TSmartPtr.h"
#include "istd/CStaticServicesProvider.h"
#include "icomp/CComponentBase.h"
#include "ibase/TLoggerCompWrap.h"

#include "iproc/TSyncProcessorCompBase.h"

#include "iimg/CGeneralBitmap.h"



namespace iipr
{


/**
	Basic implementation for a simple bitmap processor.
*/
template <class ParameterType>
class TImageProcessorCompBase: public iproc::CSyncProcessorCompBase
{
public:
	typedef iproc::CSyncProcessorCompBase BaseClass;

	TImageProcessorCompBase();

	I_BEGIN_BASE_COMPONENT(TImageProcessorCompBase)
		I_ASSIGN(m_paramsIdAttrPtr, "ParamsId", "ID of processor parameter", true, "ParamsId");
		I_ASSIGN(m_preprocessorCompPtr, "Preprocessor", "Pre-processing operator", false, "Preprocessor");
	I_END_COMPONENT

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr);

protected:
	// abstract methods
	virtual bool ProcessImage(
				const ParameterType* parameterPtr, 
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage) = 0;

private:
	istd::TSmartPtr<isys::ITimer> m_timerPtr;

	I_ATTR(istd::CString, m_paramsIdAttrPtr);
	I_REF(iproc::IProcessor, m_preprocessorCompPtr);
};


// public methods

template <class ParameterType>
TImageProcessorCompBase<ParameterType>::TImageProcessorCompBase()
:	m_timerPtr(istd::CreateService<isys::ITimer>())
{
}


// reimplemented (iproc::IProcessor)

template <class ParameterType>
int TImageProcessorCompBase<ParameterType>::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);

	if (		(inputBitmapPtr == NULL) ||
				(outputBitmapPtr == NULL) ||
				(paramsPtr == NULL) ||
				!m_paramsIdAttrPtr.IsValid()){
		return TS_INVALID;
	}

	if (m_timerPtr.IsValid()){
		m_timerPtr->Start();
	}

	iimg::CGeneralBitmap bufferBitmap;

	if (m_preprocessorCompPtr.IsValid()){
		int retVal = m_preprocessorCompPtr->DoProcessing(paramsPtr, inputBitmapPtr, &bufferBitmap);
		if (retVal == TS_OK){
			inputBitmapPtr = &bufferBitmap;
		}
	}

	// create output image:
	outputBitmapPtr->CopyImageFrom(*inputBitmapPtr);

	const ParameterType* processorParamsPtr = NULL;	
	if (paramsPtr != NULL){
		processorParamsPtr = dynamic_cast<const ParameterType*>(paramsPtr->GetParameter(m_paramsIdAttrPtr->GetValue().ToString()));
	}
	
	// do image processing:
	if (!ProcessImage(processorParamsPtr, *inputBitmapPtr, *outputBitmapPtr)){
		return TS_INVALID;
	}
	
	if (m_timerPtr.IsValid()){
		double processingTime = m_timerPtr->GetElapsed();

		SendInfoMessage(0, istd::CString("Processed in ") + istd::CString::FromNumber(processingTime * 1000) + "ms");
	}

	return TS_OK;
}


} // namespace iipr


#endif // !iipr_TImageProcessorCompBase_included

