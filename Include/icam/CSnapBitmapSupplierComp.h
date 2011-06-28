#ifndef icam_CSnapBitmapSupplierComp_included
#define icam_CSnapBitmapSupplierComp_included


// ACF includes
#include "istd/TDelPtr.h"
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
class CSnapBitmapSupplierComp: public iproc::TSupplierCompWrap<iipr::IBitmapSupplier, istd::TDelPtr<iimg::IBitmap> >
{
public:
	typedef iproc::TSupplierCompWrap<iipr::IBitmapSupplier, istd::TDelPtr<iimg::IBitmap> > BaseClass;

	I_BEGIN_COMPONENT(CSnapBitmapSupplierComp);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_bitmapAcquisitionCompPtr, "BitmapAcquisition", "Bitmap acquision object for image snap", true, "BitmapAcquisition");
	I_END_COMPONENT;

	// reimplemented (iipr::IBitmapSupplier)
	virtual const iimg::IBitmap* GetBitmap() const;

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(istd::TDelPtr<iimg::IBitmap>& result) const;

private:
	I_FACT(iimg::IBitmap, m_bitmapCompFact);

	I_REF(iproc::IBitmapAcquisition, m_bitmapAcquisitionCompPtr);
};


} // namespace icam


#endif // !icam_CSnapBitmapSupplierComp_included


