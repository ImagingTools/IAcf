#ifndef iqtipr_CProjectionShape_included
#define iqtipr_CProjectionShape_included


// Qt includes
#include <QGraphicsPathItem>


// ACF includes
#include "imod/TSingleModelObserverBase.h"

#include "iqt2d/iqt2d.h"


// IACF includes
#include "iipr/CProjectionData.h"


namespace iqtipr
{


class CProjectionShape: public QGraphicsPathItem, public imod::TSingleModelObserverBase<iipr::CProjectionData>
{
public:
	typedef imod::TSingleModelObserverBase<iipr::CProjectionData> BaseClass;

	CProjectionShape();

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
};


} // namespace iqtipr


#endif // !iqtipr_CProjectionShape_included


