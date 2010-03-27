#include "icomp/export.h"

#include "QtProdPck.h"


namespace QtProdPck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE("QtProd", "Qt-based Production and control package", "Qt Production Automatic Controller");

I_EXPORT_COMPONENT(
			DriverController,
			"Synchronize driver with production line parameter and control working mode",
			"Production Line Automatic Driver Controller",
			icomp::IComponentStaticInfo::CCT_DATA_PROCESSING);

I_EXPORT_COMPONENT(
			AutomaticControllerGui,
			"GUI for automatic controller implemented as observer",
			"Production Line Automatic Driver Controller Observer",
			icomp::IComponentStaticInfo::CCT_GUI);

I_EXPORT_COMPONENT(
			SimulatedMultiTracerGui,
			"Simulated multi line object tracer delegating controlled using GUI",
			"Sumulation Multi Line Object Tracer Qt GUI",
			icomp::IComponentStaticInfo::CCT_GUI);


} // namespace QtProdPck


