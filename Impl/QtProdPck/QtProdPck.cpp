#include "icomp/export.h"

#include "QtProdPck.h"


namespace QtProdPck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE(
			"QtProd",
			"Qt-based production and control package",
			"Qt Production Automatic Controller" IM_PROJECT("\"ACF Computer Vision\"") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			DriverController,
			"Synchronize driver with production line parameter and control working mode",
			"Production Line Automatic Driver Controller");

I_EXPORT_COMPONENT(
			AutomaticControllerGui,
			"GUI for automatic controller implemented as observer",
			"Production Line Automatic Driver Controller Observer");

I_EXPORT_COMPONENT(
			SimulatedMultiTracerGui,
			"Simulated multi line object tracer delegating controlled using GUI",
			"Sumulation Multi Line Object Tracer Qt GUI");


} // namespace QtProdPck


