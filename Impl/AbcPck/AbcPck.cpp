#include "AbcPck.h"


#include "icomp/export.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
	}

} instance;


namespace AbcPck
{


I_EXPORT_PACKAGE(
			"Abc",
			"Application Business Components",
			"Application Business Enterprise" IM_PROJECT("ACF-Solutions") IM_TAG("Qt") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ServiceApplication,
			"An application as service/daemon",
			"Service Daemon" IM_TAG("Application"));


} // namespace AbcPck


