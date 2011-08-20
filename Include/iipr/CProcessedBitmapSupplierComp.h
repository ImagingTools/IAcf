#ifndef iipr_CProcessedBitmapSupplierComp_included
#define iipr_CProcessedBitmapSupplierComp_included


// STL includes
#include <utility>

// ACF includes
#include "iproc/IProcessor.h"
#include "iproc/TSupplierCompWrap.h"

// IACF includes
#include "iipr/IBitmapSupplier.h"


namespace iipr
{


/**
	Image supplier providing processed image from some other input image supplier.
*/
class CProcessedBitmapSupplierComp: public iproc::TSupplierCompWrap<iipr::IBitmapSupplier, std::pair<istd::TDelPtr<i2d::ITransformation2d>,  istd::TDelPtr<iimg::IBitmap> > >
{
public:
	typedef iproc::TSupplierCompWrap<iipr::IBitmapSupplier, std::pair<istd::TDelPtr<i2d::ITransformation2d>,  istd::TDelPtr<iimg::IBitmap> > > BaseClass;

	I_BEGIN_COMPONENT(CProcessedBitmapSupplierComp);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_inputBitmapSupplierCompPtr, "BitmapSupplier", "Provide input image", true, "BitmapSupplier");
		I_ASSIGN(m_imageProcessorCompPtr, "BitmapProcessor", "Bitmap conversion processor (takes bitmap as input and output)", true, "BitmapProcessor");
	I_END_COMPONENT;

protected:
	bool EnsureBitmapCreated(ProductType& result) const;

	// reimplemented (iipr::IBitmapSupplier)
	virtual const iimg::IBitmap* GetBitmap() const;
	virtual const i2d::ITransformation2d* GetLogTransform() const;

	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_FACT(iimg::IBitmap, m_bitmapCompFact);

	I_REF(iipr::IBitmapSupplier, m_inputBitmapSupplierCompPtr);
	I_REF(iproc::IProcessor, m_imageProcessorCompPtr);
};


} // namespace iipr


#endif // !iipr_CProcessedBitmapSupplierComp_included


