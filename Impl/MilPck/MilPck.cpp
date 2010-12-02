#include "icomp/export.h"

#include "MilPck.h"


namespace MilPck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE(
			"MilPck",
			"MIL (Matrox Imaging Library) integration",
			"MIL Qt \"Model Search\"" IM_PROJECT("IACF") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			MilSearchParamsGui,
			"Search params GUI",
			"Search parameter GUI MIL");

I_EXPORT_COMPONENT(
			MilSearchParams,
			"Search parameters for MIL search",
			"Search parameter GUI MIL");

I_EXPORT_COMPONENT(
			MilSearchProcessor,
			"Search processor",
			"Search parameter GUI MIL");


} // namespace MilPck
