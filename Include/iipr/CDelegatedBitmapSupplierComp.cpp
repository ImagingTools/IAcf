#include "iipr/CDelegatedBitmapSupplierComp.h"


namespace iipr
{


// reimplemented (iipr::IBitmapSupplier)

const iimg::IBitmap* CDelegatedBitmapSupplierComp::GetBitmap() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->second;
	}

	return NULL;
}


const i2d::ITransformation2d* CDelegatedBitmapSupplierComp::GetLogTransform() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->first;
	}
	else{
		return NULL;
	}
}


//protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CDelegatedBitmapSupplierComp::ProduceObject(ProductType& result) const
{
	if (m_inputBitmapSupplierCompPtr.IsValid()){
		result.first = m_inputBitmapSupplierCompPtr->GetLogTransform();
		result.second = m_inputBitmapSupplierCompPtr->GetBitmap();

		return m_inputBitmapSupplierCompPtr->GetWorkStatus();
	}

	if (m_bitmapCompPtr.IsValid()){
		result.first = m_calibrationCompPtr.GetPtr();
		result.second = m_bitmapCompPtr.GetPtr();

		return ISupplier::WS_OK;
	}

	return ISupplier::WS_CRITICAL;
}


// reimplemented (icomp::CComponentBase)

void CDelegatedBitmapSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_inputBitmapSupplierCompPtr.IsValid()){
		AddInputSupplier(m_inputBitmapSupplierCompPtr.GetPtr());
	}
	else{
		if (m_bitmapModelCompPtr.IsValid()){
			m_bitmapModelCompPtr->AttachObserver(this);
		}
		if (m_calibrationModelCompPtr.IsValid()){
			m_calibrationModelCompPtr->AttachObserver(this);
		}
	}
}


void CDelegatedBitmapSupplierComp::OnComponentDestroyed()
{
	if (m_inputBitmapSupplierCompPtr.IsValid()){
		RemoveInputSupplier(m_inputBitmapSupplierCompPtr.GetPtr());
	}
	else{
		EnsureModelsDetached();
	}

	BaseClass::OnComponentDestroyed();
}


// reimplemented (imod::CSingleModelObserverBase)

void CDelegatedBitmapSupplierComp::OnUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	InvalidateSupplier();
}


} // namespace iipr


