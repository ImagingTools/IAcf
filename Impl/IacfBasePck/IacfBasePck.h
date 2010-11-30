#ifndef IacfBasePck_included
#define IacfBasePck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "icam/CExposureParamsComp.h"
#include "icam/CTriggerParamsComp.h"
#include "icam/CSnapBitmapSupplierComp.h"
#include "icam/CMemoryAcquisitionComp.h"
#include "icam/CAcquisitionConstraintsComp.h"

#include "isig/CSamplingParamsComp.h"

#include "iinsp/CInspectionTaskComp.h"


/**
	Base system-undependent general package.
*/
namespace IacfBasePck
{


typedef icomp::TModelCompWrap<icam::CExposureParamsComp> ExposureParams;
typedef icomp::TModelCompWrap<icam::CTriggerParamsComp> TriggerParams;
typedef icomp::TModelCompWrap<icam::CSnapBitmapSupplierComp> SnapBitmapSupplier;
typedef icam::CMemoryAcquisitionComp MemoryAcquisition;
typedef icam::CAcquisitionConstraintsComp AcquisitionConstraints;
typedef icomp::TModelCompWrap<isig::CSamplingParamsComp> SamplingParams;
typedef icomp::TModelCompWrap<iinsp::CInspectionTaskComp> InspectionTask;


} // namespace IacfBasePck


#endif // !IacfBasePck_included


