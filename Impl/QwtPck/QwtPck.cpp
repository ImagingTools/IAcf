#include "QwtPck.h"


#include "icomp/export.h"


namespace QwtPck
{


I_REGISTER_QT_RESOURCE(iqtgui);
I_REGISTER_QT_RESOURCE(Qwt3d);

I_EXPORT_PACKAGE(
			"Iacf/Gui/Qwt",
			"QWT and QWT3D based package",
			"Qwt Qt 2D 3D View" IM_TAG("Qt") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ProgressHistoryGui,
			"Progress history shown as 2D diagram",
			"Progress History Diagram Activity Curve Line Statistics" IM_TAG("2D"));

I_EXPORT_COMPONENT(
			HistogramView,
			"Histogram view",
			"Histogram Statistics" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			SurfaceView,
			"3D-View component",
			"Model Observer Parameter GUI Qt 3D Surface" IM_TAG("Image 3D"));


} // namespace QwtPck


