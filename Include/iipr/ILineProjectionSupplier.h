#ifndef iipr_ILineProjectionSupplier_included
#define iipr_ILineProjectionSupplier_included


// ACF includes
#include "iproc/ISupplier.h"

// ACF-Solutions includes
#include "imeas/IDataSequence.h"

#include "iipr/iipr.h"


namespace iipr
{


/**
	Image line projection supplier.
*/
class ILineProjectionSupplier: virtual public iproc::ISupplier
{
public:
	/**
		Get access to produced line projection object.
	*/
	virtual const imeas::IDataSequence* GetLineProjection() const = 0;
};


} // namespace iipr


#endif // !iipr_ILineProjectionSupplier_included


