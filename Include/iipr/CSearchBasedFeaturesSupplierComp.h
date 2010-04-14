#ifndef iipr_CSearchBasedFeaturesSupplierComp_included
#define iipr_CSearchBasedFeaturesSupplierComp_included


#include "i2d/CVector2d.h"

#include "iprm/IParamsSet.h"

#include "iproc/IProcessor.h"
#include "iproc/TSupplierCompWrap.h"

#include "iproc/IBitmapSupplier.h"

#include "iipr/IFeaturesSupplier.h"
#include "iipr/TFeaturesContainerWrap.h"
#include "iipr/CSearchFeature.h"


namespace iipr
{


typedef iipr::TFeaturesContainerWrap<iipr::IFeaturesContainer, iipr::CSearchFeature> CSearchFeatures;


class CSearchBasedFeaturesSupplierComp: public iproc::TSupplierCompWrap<iipr::IFeaturesSupplier, CSearchFeatures>
{
public:
	typedef iproc::TSupplierCompWrap<iipr::IFeaturesSupplier, CSearchFeatures> BaseClass;

	I_BEGIN_COMPONENT(CSearchBasedFeaturesSupplierComp);
		I_REGISTER_INTERFACE(CSearchBasedFeaturesSupplierComp);
		I_ASSIGN(m_bitmapSupplierCompPtr, "BitmapSupplier", "Provide image to analyse", true, "BitmapSupplier");
		I_ASSIGN(m_searchProcessorCompPtr, "SearchProcessor", "Calculate model positions in the image", true, "SearchProcessor");
	I_END_COMPONENT;

	// reimplemented (iipr::IFeaturesSupplier)
	virtual const iipr::IFeaturesContainer* GetFeatures() const;

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(CSearchFeatures& result) const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	I_REF(iproc::IBitmapSupplier, m_bitmapSupplierCompPtr);
	I_REF(iproc::IProcessor, m_searchProcessorCompPtr);
};


} // namespace iipr


#endif // !iipr_CSearchBasedFeaturesSupplierComp_included


