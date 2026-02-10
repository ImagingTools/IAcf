// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-IAcf-Commercial
#include "PhononPck.h"


#include <icomp/export.h>


namespace PhononPck
{


using namespace icomp;


I_EXPORT_PACKAGE(
			"Iacf/Phonon",
			"Phonon based package",
			"Media" IM_PROJECT("ACF") IM_TAG("Qt Multimedia") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			PhononVideoViewGui,
			"Video widget based on Phonon library",
			"Phonon Widget Video Play" IM_TAG("GUI"));


} // namespace PhononPck


