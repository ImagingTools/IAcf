#include "icomp/export.h"

#include "MePck.h"


namespace MePck
{


I_EXPORT_PACKAGE("MePck", "Standard Meilhaus hardware package based on Qt", "Meilhaus Qt Standard");

I_EXPORT_COMPONENT(
			SamplingParams,
			"Sampling params based on fixed boundary values",
			"Sample Sampling Acquisition Model Parameters",
			icomp::IComponentStaticInfo::CCT_DATA);

I_EXPORT_COMPONENT(
			MeilhausSimpleAcquisition,
			"Simple implementation of sample aquisition using Meilhaus MEIDS API",
			"Simple Sample Acquisition Meids Osciloscope Meilhaus Analog IO Card",
			icomp::IComponentStaticInfo::CCT_DATA_PROCESSING);

I_EXPORT_COMPONENT(
			ChannelAddressParam,
			"Storing address of selected IO channel",
			"Channel Address Param Selection Meilhaus",
			icomp::IComponentStaticInfo::CCT_DATA);


} // namespace MePck


