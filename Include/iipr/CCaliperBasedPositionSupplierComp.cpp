#include "iipr/CCaliperBasedPositionSupplierComp.h"


#include "iipr/CHeaviestFeatureConsumer.h"


namespace iipr
{


// reimplemented (iproc::IValueSupplier)

imath::CVarVector CCaliperBasedPositionSupplierComp::GetValue(int /*index*/, int /*valueTypeId*/) const
{
	const i2d::CVector2d* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return *productPtr;
	}
	else{
		return imath::CVarVector();
	}
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CCaliperBasedPositionSupplierComp::ProduceObject(i2d::CVector2d& result) const
{
	if (		m_bitmapSupplierCompPtr.IsValid() &&
				m_featuresMapperCompPtr.IsValid() &&
				m_caliperToolCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapSupplierCompPtr->GetBitmap();
		if (bitmapPtr != NULL){
			iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();

			CHeaviestFeatureConsumer consumer;
			int caliperState = m_caliperToolCompPtr->DoProcessing(
							paramsSetPtr,
							bitmapPtr,
							&consumer);

			if (caliperState != iproc::IProcessor::TS_OK){
				return WS_ERROR;
			}

			const IFeature* featurePtr = consumer.GetFeature();
			if (featurePtr == NULL){
				return WS_ERROR;
			}

			if (m_featuresMapperCompPtr->GetImagePosition(
						*featurePtr,
						paramsSetPtr,
						result)){
				return WS_OK;
			}
		}
	}

	return WS_CRITICAL;
}


// reimplemented (icomp::IComponent)

void CCaliperBasedPositionSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	AddInputSupplier(m_bitmapSupplierCompPtr.GetPtr());
}


} // namespace iipr


