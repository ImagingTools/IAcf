#ifndef QtProdPck_included
#define QtProdPck_included


#include "iqtcntl/CDriverControllerComp.h"
#include "iqtcntl/CAutomaticControllerGuiComp.h"
#include "iqtcntl/CSimulatedMultiTracerGuiComp.h"


/**
	Qt camera package.
*/
namespace QtProdPck
{


typedef iqtcntl::CDriverControllerComp DriverController;

// GUI components
typedef iqtcntl::CAutomaticControllerGuiComp AutomaticControllerGui;
typedef iqtcntl::CSimulatedMultiTracerGuiComp SimulatedMultiTracerGui;


} // namespace QtProdPck


#endif // !QtProdPck_included


