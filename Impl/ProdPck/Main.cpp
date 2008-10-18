#include "icomp/export.h"

#include "ProdPck.h"


namespace ProdPck
{


I_EXPORT_PACKAGE("Prod", "Production and control package", "Production Automatic Controller");

I_EXPORT_COMPONENT(MultiLineParams, "Parameter set for multi line production system", "Production Multi Lines Automatic Controller");
I_EXPORT_COMPONENT(LineParams, "Parameter set for sinle line of production system", "Production Line Automatic Controller");
I_EXPORT_COMPONENT(InspectionUnitParams, "Parameter set for sinle inspection unit of production system", "Production Inspection Unit Automatic Controller");
I_EXPORT_COMPONENT(EjectorParams, "Parameter set for sinle ejector for production system", "Production Inspection Ejector Automatic Controller");


} // namespace ProdPck


