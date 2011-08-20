#include "iipr/CProcessedBitmapSupplierComp.h"

// ACF includes
#include "istd/IPolymorphic.h"


namespace iipr
{


// reimplemented (iipr::IBitmapSupplier)

const iimg::IBitmap* CProcessedBitmapSupplierComp::GetBitmap() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->second.GetPtr();
	}

	return NULL;
}


const i2d::ITransformation2d* CProcessedBitmapSupplierComp::GetLogTransform() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->first.GetPtr();
	}
	else{
		return NULL;
	}
}


//protected methods

bool CProcessedBitmapSupplierComp::EnsureBitmapCreated(ProductType& result) const
{
	if (!m_bitmapCompFact.IsValid()){
		return false;
	}

	if (!result.second.IsValid()){
		result.second.SetPtr(m_bitmapCompFact.CreateInstance());
	}

	return result.second.IsValid();
}


// reimplemented (iproc::TSupplierCompWrap)

int CProcessedBitmapSupplierComp::ProduceObject(ProductType& result) const
{
	if (!m_inputBitmapSupplierCompPtr.IsValid() || !m_imageProcessorCompPtr.IsValid()){
		return WS_CRITICAL;
	}

	if (!EnsureBitmapCreated(result)){
		return WS_CRITICAL;
	}
	I_ASSERT(result.second.IsValid());

	const iimg::IBitmap* bitmapPtr = m_inputBitmapSupplierCompPtr->GetBitmap();
	if (bitmapPtr == NULL){
		return WS_ERROR;
	}

	int status = m_imageProcessorCompPtr->DoProcessing(GetModelParametersSet(), bitmapPtr, result.second.GetPtr());
	switch (status){
	case iproc::IProcessor::TS_OK:
		return WS_OK;

	case iproc::IProcessor::TS_CANCELED:
		return WS_CANCELED;

	default:
		return WS_ERROR;
	}
}


// reimplemented (icomp::CComponentBase)

void CProcessedBitmapSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_inputBitmapSupplierCompPtr.IsValid()){
		AddInputSupplier(m_inputBitmapSupplierCompPtr.GetPtr());
	}
}


void CProcessedBitmapSupplierComp::OnComponentDestroyed()
{
	if (m_inputBitmapSupplierCompPtr.IsValid()){
		RemoveInputSupplier(m_inputBitmapSupplierCompPtr.GetPtr());
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace iipr


