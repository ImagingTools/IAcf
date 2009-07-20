#include "iipr/CLineProjectionSupplierComp.h"


#include "iser/CMemoryReadArchive.h"

#include "iipr/CRectDerivativeProcessor.h"


namespace iipr
{


// reimplemented (iproc::IValueSupplier)

const iipr::CProjectionData* CLineProjectionSupplierComp::GetLineProjection(I_DWORD objectId) const
{
	const WorkInfo* infoPtr = GetWorkInfo(objectId, true);
	if ((infoPtr != NULL) && (infoPtr->status == WS_OK)){
		return infoPtr->product.GetPtr();
	}

	return NULL;
}


// reimplemented (iproc::ISupplier)

void CLineProjectionSupplierComp::BeginNextObject(I_DWORD objectId)
{
	if (!IsIdKnown(objectId)){
		BaseClass::BeginNextObject(objectId);

		if (m_bitmapSupplierCompPtr.IsValid()){
			m_bitmapSupplierCompPtr->BeginNextObject(objectId);
		}
	}
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CLineProjectionSupplierComp::ProduceObject(I_DWORD objectId, istd::TDelPtr<iipr::CProjectionData>& result) const
{
	if (!result.IsValid()){
		result.SetPtr(new iipr::CProjectionData);
	}

	if (		m_bitmapSupplierCompPtr.IsValid() &&
				m_projectionProcessorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapSupplierCompPtr->GetBitmap(objectId);
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


} // namespace iipr


