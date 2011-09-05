#ifndef iipr_IMultiBitmapSupplier_included
#define iipr_IMultiBitmapSupplier_included


// ACF includes
#include "i2d/ITransformation2d.h"
#include "iimg/IBitmap.h"
#include "iproc/ISupplier.h"


namespace iipr
{


/**
	Supplier allowing to access of multiple produced bitmap.
*/
class IMultiBitmapSupplier: virtual public iproc::ISupplier
{
public:
	/**
		Get number of produced bitmaps.
	*/
	virtual int GetBitmapsCount() const = 0;
	/**
		Get access to produced bitmap object.
		\return	pointer to bitmap instance if this bitmap is accessible, or NULL.
	*/
	virtual const iimg::IBitmap* GetBitmap(int bitmapIndex) const = 0;

	/**
		Get access to transformation from bitmap to logical positions.
		Please note, not all implementations must support this information.
		\return	pointer to transformation if it is accessible, or NULL.
	*/
	virtual const i2d::ITransformation2d* GetLogTransform(int bitmapIndex) const = 0;
};


} // namespace iipr


#endif // !iipr_IMultiBitmapSupplier_included


