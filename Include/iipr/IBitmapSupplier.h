#ifndef iipr_IBitmapSupplier_included
#define iipr_IBitmapSupplier_included


#include "istd/CIndex2d.h"

#include "iimg/IBitmap.h"

#include "iproc/ISupplier.h"


namespace iipr
{


/**
	Bitmap supplier allowing to access of produced bitmap.
*/
class IBitmapSupplier: virtual public iproc::ISupplier
{
public:
	/**
		Get access to produced bitmap object.
		\return	pointer to bitmap instance if this bitmap is accessible, or NULL.
	*/
	virtual const iimg::IBitmap* GetBitmap() const = 0;
};


} // namespace iipr


#endif // !iipr_IBitmapSupplier_included

