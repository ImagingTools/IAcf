#include "iipr/CSearchBasedFeaturesSupplierComp.h"


namespace iipr
{


// reimplemented (iipr::IFeaturesSupplier)

const iipr::IFeaturesContainer* CSearchBasedFeaturesSupplierComp::GetFeatures() const
{
	return GetWorkProduct();
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CSearchBasedFeaturesSupplierComp::ProduceObject(CFeaturesContainer& result) const
{
	if (		m_bitmapSupplierCompPtr.IsValid() &&
				m_searchProcessorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapSupplierCompPtr->GetBitmap();
		if (bitmapPtr != NULL){
			iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();

			int searchState = m_searchProcessorCompPtr->DoProcessing(
							paramsSetPtr,
							bitmapPtr,
							&result);

			if (searchState != iproc::IProcessor::TS_OK){
				return WS_ERROR;
			}

			return WS_OK;
		}
	}

	return WS_CRITICAL;
}


// reimplemented (icomp::IComponent)

void CSearchBasedFeaturesSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	AddInputSupplier(m_bitmapSupplierCompPtr.GetPtr());
}


} // namespace iipr


