#include "icam/CSnapBitmapSupplierComp.h"


namespace icam
{


// reimplemented (iipr::IBitmapSupplier)

const iimg::IBitmap* CSnapBitmapSupplierComp::GetBitmap() const
{
	const istd::TDelPtr<iimg::IBitmap>* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->GetPtr();
	}
	else{
		return NULL;
	}
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CSnapBitmapSupplierComp::ProduceObject(istd::TDelPtr<iimg::IBitmap>& result) const
{
	if (m_bitmapCompFact.IsValid() && !result.IsValid()){
		result.SetPtr(m_bitmapCompFact.CreateInstance());
	}

	if (result.IsValid() && m_bitmapAcquisitionCompPtr.IsValid()){
		int status = m_bitmapAcquisitionCompPtr->DoProcessing(GetModelParametersSet(), NULL, result.GetPtr());
		switch (status){
		case iproc::IProcessor::TS_OK:
			return WS_OK;

		case iproc::IProcessor::TS_CANCELED:
			return WS_CANCELED;

		default:
			return WS_ERROR;
		}
	}

	return WS_CRITICAL;
}


} // namespace icam

