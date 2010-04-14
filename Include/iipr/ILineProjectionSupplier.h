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
	*/
	virtual const iipr::CProjectionData* GetLineProjection() const = 0;
};


} // namespace iipr


#endif // !iipr_ILineProjectionSupplier_included


