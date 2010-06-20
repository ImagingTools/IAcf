#include "iipr/CLineProjectionSupplierComp.h"


#include "iser/CMemoryReadArchive.h"

#include "iipr/CRectDerivativeProcessor.h"


namespace iipr
{


// reimplemented (iproc::IValueSupplier)

const imeas::IDataSequence* CLineProjectionSupplierComp::GetLineProjection() const
{
	return GetWorkProduct();
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CLineProjectionSupplierComp::ProduceObject(imeas::CGeneralDataSequence& result) const
{
	if (		m_bitmapSupplierCompPtr.IsValid() &&
				m_projectionProcessorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapSupplierCompPtr->GetBitmap();
		if (bitmapPtr != NULL){
			iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();

			int projectionState = m_projectionProcessorCompPtr->DoProcessing(
						paramsSetPtr,
						bitmapPtr,
						&result);
			if (projectionState == iproc::IProcessor::TS_OK){
				return WS_OK;
			}
		}

		return WS_ERROR;
	}

	return WS_CRITICAL;
}


// reimplemented (icomp::IComponent)

void CLineProjectionSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	AddInputSupplier(m_bitmapSupplierCompPtr.GetPtr());
}


} // namespace iipr


