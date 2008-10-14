#ifndef ProdPck_included
#define ProdPck_included


#include "imod/TModelWrap.h"

#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "icntl/CDriverControllerComp.h"


/**
	System-independent production control package.
*/
namespace ProdPck
{

typedef icntl::CDriverControllerComp DriverController;


} // namespace ProdPck


#endif // !ProdPck_included


