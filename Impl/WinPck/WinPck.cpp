// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-IAcf-Commercial
#include "WinPck.h"


#include <icomp/export.h>


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


