#ifndef ProdPck_included
#define ProdPck_included


#include "imod/TModelWrap.h"

#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "icntl/CDriverControllerComp.h"
#include "icntl/CMultiLineParamsComp.h"
#include "icntl/CLineParamsComp.h"
#include "icntl/CInspectionUnitParamsComp.h"
#include "icntl/CEjectorParamsComp.h"


/**
	System-independent production control package.
*/
namespace ProdPck
{

typedef icntl::CDriverControllerComp DriverController;
typedef icomp::TModelCompWrap<icntl::CMultiLineParamsComp> MultiLineParams;
typedef icomp::TModelCompWrap<icntl::CLineParamsComp> LineParams;
typedef icomp::TModelCompWrap<icntl::CInspectionUnitParamsComp> InspectionUnitParams;
typedef icomp::TModelCompWrap<icntl::CEjectorParamsComp> EjectorParams;


} // namespace ProdPck


#endif // !ProdPck_included


