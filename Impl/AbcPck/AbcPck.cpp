#include "AbcPck.h"


#include "icomp/export.h"


namespace AbcPck
{


I_REGISTER_QT_RESOURCE(iqtgui);

I_EXPORT_PACKAGE(
			"Iacf/Abc",
			"Application Business Components",
			"Application Business Enterprise" IM_PROJECT("ACF-Solutions") IM_TAG("Qt") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ServiceApplication,
			"An application as service/daemon",
			"Service Daemon" IM_TAG("Application"));


} // namespace AbcPck


