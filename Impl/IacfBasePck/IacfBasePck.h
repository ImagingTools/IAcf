#ifndef IacfBasePck_included
#define IacfBasePck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iproc/CTestIdManagerComp.h"
#include "iproc/CCascadedProcessorComp.h"

#include "icam/CExposureParamsComp.h"
#include "icam/CSnapBitmapSupplierComp.h"
#include "icam/CMemoryAcquisitionComp.h"


/**
	Base system-undependent general package.
*/
namespace IacfBasePck
{


typedef iproc::CTestIdManagerComp TestIdManager;
typedef iproc::CCascadedProcessorComp CascadedProcessor;

typedef icomp::TModelCompWrap<icam::CExposureParamsComp> ExposureParams;
typedef icomp::TModelCompWrap<icam::CSnapBitmapSupplierComp> SnapBitmapSupplier;
typedef icam::CMemoryAcquisitionComp MemoryAcquisition;


} // namespace IacfBasePck


#endif // !IacfBasePck_included


