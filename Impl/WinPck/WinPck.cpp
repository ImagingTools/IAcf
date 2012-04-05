#include "icomp/export.h"

#include "WinPck.h"


namespace WinPck
{


I_EXPORT_PACKAGE(
			"Iacf/Windows",
			"Widows dependend standard package",
			"Win32 Windows" IM_PROJECT("IACF") IM_TAG("General") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\""));


I_EXPORT_COMPONENT(
			TimeStampCache,
			"Implementation of time stamp provider used to hold a copy of another provider",
			"Time Stamp Provider Copy Cache");


} // namespace WinPck


