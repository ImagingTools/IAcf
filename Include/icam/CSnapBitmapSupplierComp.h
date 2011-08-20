#ifndef icam_CSnapBitmapSupplierComp_included
#define icam_CSnapBitmapSupplierComp_included


// ACF includes
#include "istd/TDelPtr.h"
#include "i2d/ITransformation2d.h"
#include "iimg/IBitmap.h"
#include "iprm/IParamsSet.h"
#include "iproc/TSupplierCompWrap.h"
#include "iproc/IBitmapAcquisition.h"

#include "iipr/IBitmapSupplier.h"


namespace icam
{


/**
	Implementation of bitmap supplier based on image acquisition.
*/
class CSnapBitmapSupplierComp: public iproc::TSupplierCompWrap<iipr::IBitmapSupplier, std::pair<istd::TDelPtr<const i2d::ITransformation2d>,  istd::TDelPtr<iimg::IBitmap> > >
{
public:
	typedef iproc::TSupplierCompWrap<iipr::IBitmapSupplier, std::pair<istd::TDelPtr<const i2d::ITransformation2d>,  istd::TDelPtr<iimg::IBitmap> > > BaseClass;

	I_BEGIN_COMPONENT(CSnapBitmapSupplierComp);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_bitmapAcquisitionCompPtr, "BitmapAcquisition", "Bitmap acquision object for image snap", true, "BitmapAcquisition");
		I_ASSIGN(m_calibrationCompPtr, "Calibration", "Optional calibration object", false, "Calibration");
	I_END_COMPONENT;

	// reimplemented (iipr::IBitmapSupplier)
	virtual const iimg::IBitmap* GetBitmap() const;
	virtual const i2d::ITransformation2d* GetLogTransform() const;

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

private:
	I_FACT(iimg::IBitmap, m_bitmapCompFact);

	I_REF(iproc::IBitmapAcquisition, m_bitmapAcquisitionCompPtr);
	I_REF(i2d::ITransformation2d, m_calibrationCompPtr);
};


} // namespace icam


#endif // !icam_CSnapBitmapSupplierComp_included


