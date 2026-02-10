// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-IAcf-Commercial
#include "QwtPck.h"


#include <icomp/export.h>


namespace QwtPck
{


I_EXPORT_PACKAGE(
			"Iacf/Gui/Qwt",
			"QWT and QWT3D based package",
			"Qwt Qt 2D 3D View" IM_TAG("Qt") IM_PROJECT("IACF") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ProgressHistoryGui,
			"Progress history shown as 2D diagram",
			"Progress History Diagram Activity Curve Line Statistics" IM_TAG("2D"));

I_EXPORT_COMPONENT(
			HistogramView,
			"Histogram view",
			"Histogram Statistics" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
		   DataSequenceView,
		   "Data sequence view",
		   "Data Sequence View" IM_TAG("GUI") IM_AUTHOR("\"Alexander Duda\""));

I_EXPORT_COMPONENT(
			SurfaceView,
			"3D-View component",
			"Model Observer Parameter GUI Qt 3D Surface" IM_TAG("Image 3D"));


} // namespace QwtPck


