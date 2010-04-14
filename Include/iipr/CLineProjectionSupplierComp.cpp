#include "iipr/CLineProjectionSupplierComp.h"


#include "iser/CMemoryReadArchive.h"

#include "iipr/CRectDerivativeProcessor.h"


namespace iipr
{


// reimplemented (iproc::IValueSupplier)

const iipr::CProjectionData* CLineProjectionSupplierComp::GetLineProjection() const
{
	const istd::TDelPtr<iipr::CProjectionData>* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->GetPtr();
	}
	else{
		return NULL;
	}
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CLineProjectionSupplierComp::ProduceObject(istd::TDelPtr<iipr::CProjectionData>& result) const
{
	if (!result.IsValid()){
		result.SetPtr(new iipr::CProjectionData);
	}

	if (		m_bitmapSupplierCompPtr.IsValid() &&
				m_projectionProcessorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapSupplierCompPtr->GetBitmap();
		if (bitmapPtr != NULL){
			iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();

			int projectionState = m_projectionProcessorCompPtr->DoProcessing(
							paramsSetPtr,
							bitmapPtr,
							result.GetPtr());

			if (projectionState != iproc::IProcessor::TS_OK){
				return WS_ERROR;
			}

/*			iipr::CRectDerivativeProcessor derivative;
			iipr::CProjectionData output;
			i2d::CLine2d projectionLine = result->GetProjectionLine();
			derivative.DoDerivativeProcessing(*result.GetPtr(), 5, output);

			iser::CMemoryReadArchive::CloneObjectByArchive(output, *result.GetPtr());
			result->SetProjectionLine(projectionLine);
*/
			return WS_OK;
		}
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


