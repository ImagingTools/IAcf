#ifndef iipr_ILineProjectionSupplier_included
#define iipr_ILineProjectionSupplier_included


#include "iproc/ISupplier.h"

#include "iipr/CProjectionData.h"


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
		\param	objectId	ID of inspected object. \sa	iproc::ISupplier::BeginNextObject.
	*/
	virtual const iipr::CProjectionData* GetLineProjection(I_DWORD objectId) const = 0;
};


} // namespace iipr


#endif // !iipr_ILineProjectionSupplier_included


