#include "icomp/export.h"

#include "MePck.h"


namespace MePck
{


I_EXPORT_PACKAGE(
			"MePck",
			"Standard Meilhaus hardware package based on Qt",
			"Meilhaus Qt Standard" IM_PROJECT("\"ACF Computer Vision\"") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			MeilhausSimpleAcquisition,
			"Simple implementation of sample aquisition using Meilhaus MEIDS API",
			"Simple Sample Acquisition Meids Osciloscope Meilhaus Analog IO Card");

I_EXPORT_COMPONENT(
			ChannelAddressParam,
			"Storing address of selected IO channel",
			"Channel Address Param Selection Meilhaus");


} // namespace MePck


