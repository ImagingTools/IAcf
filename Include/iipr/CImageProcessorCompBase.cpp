#include "iipr/CImageProcessorCompBase.h"


// ACF includes
#include "istd/CStaticServicesProvider.h"
#include "iimg/CGeneralBitmap.h"

#include "iipr/CImageProcessorCompBase.h"


namespace iipr
{


// public methods

CImageProcessorCompBase::CImageProcessorCompBase()
:	m_timerPtr(istd::CreateService<isys::ITimer>())
{
}


// reimplemented (iproc::IProcessor)

int CImageProcessorCompBase::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* /*progressManagerPtr*/)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);

	if ((inputBitmapPtr == NULL) || (outputBitmapPtr == NULL)){
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
	outputBitmapPtr->CopyFrom(*inputBitmapPtr);

	// do image processing:
	if (!ProcessImage(paramsPtr, *inputBitmapPtr, *outputBitmapPtr)){
		return TS_INVALID;
	}
	
	if (m_timerPtr.IsValid()){
		double processingTime = m_timerPtr->GetElapsed();

		SendInfoMessage(0, istd::CString("Processed in ") + istd::CString::FromNumber(processingTime * 1000) + "ms");
	}

	return TS_OK;
}


} // namespace iipr


