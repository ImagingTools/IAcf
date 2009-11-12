#ifndef iipr_CLineProjectionSupplierComp_included
#define iipr_CLineProjectionSupplierComp_included


#include "i2d/CVector2d.h"

#include "iprm/IParamsSet.h"

#include "iproc/IProcessor.h"
#include "iproc/TSupplierCompWrap.h"

#include "iproc/IBitmapSupplier.h"

#include "iipr/ILineProjectionSupplier.h"
#include "iipr/ILineProjectionProcessor.h"


namespace iipr
{


class CLineProjectionSupplierComp: public iproc::TSupplierCompWrap<iipr::ILineProjectionSupplier, istd::TDelPtr<iipr::CProjectionData> >
{
public:
	typedef iproc::TSupplierCompWrap<iipr::ILineProjectionSupplier, istd::TDelPtr<iipr::CProjectionData> > BaseClass;

	I_BEGIN_COMPONENT(CLineProjectionSupplierComp);
		I_REGISTER_INTERFACE(CLineProjectionSupplierComp);
		I_ASSIGN(m_bitmapSupplierCompPtr, "BitmapSupplier", "Provide image to analyse", true, "BitmapSupplier");
		I_ASSIGN(m_projectionProcessorCompPtr, "ProjectionProcessor", "Processor for projection data generation", true, "ProjectionProcessor");
	I_END_COMPONENT;

	// reimplemented (iipr::ILineProjectionSupplier)
	virtual const iipr::CProjectionData* GetLineProjection(I_DWORD objectId) const;

	// reimplemented (iproc::ISupplier)
	virtual void BeginNextObject(I_DWORD objectId);

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(I_DWORD objectId, istd::TDelPtr<iipr::CProjectionData>& result) const;

private:
	I_REF(iproc::IBitmapSupplier, m_bitmapSupplierCompPtr);
	I_REF(iipr::ILineProjectionProcessor, m_projectionProcessorCompPtr);

	iipr::CProjectionData m_projectionData;
};


} // namespace iipr


#endif // !iipr_CLineProjectionSupplierComp_included


