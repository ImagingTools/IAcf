#include "iipr/CPositionFromImageSupplierComp.h"


// ACF includes
#include "i2d/CPosition2d.h"

#include "iipr/CHeaviestFeatureConsumer.h"


namespace iipr
{


// reimplemented (iproc::IValueSupplier)

imath::CVarVector CPositionFromImageSupplierComp::GetValue(int /*index*/, int /*valueTypeId*/) const
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

int CPositionFromImageSupplierComp::ProduceObject(i2d::CVector2d& result) const
{
	if (		m_bitmapSupplierCompPtr.IsValid() &&
				m_processorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapSupplierCompPtr->GetBitmap();
		if (bitmapPtr != NULL){
			iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();

			CHeaviestFeatureConsumer consumer;
			int caliperState = m_processorCompPtr->DoProcessing(
							paramsSetPtr,
							bitmapPtr,
							&consumer);

			if (caliperState != iproc::IProcessor::TS_OK){
				return WS_ERROR;
			}

			const i2d::CPosition2d* positionPtr = dynamic_cast<const i2d::CPosition2d*>(consumer.GetFeature());
			if (positionPtr == NULL){
				return WS_ERROR;
			}

			result = positionPtr->GetPosition();

			return WS_OK;
		}
	}

	return WS_CRITICAL;
}


// reimplemented (icomp::IComponent)

void CPositionFromImageSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	AddInputSupplier(m_bitmapSupplierCompPtr.GetPtr());
}


} // namespace iipr


