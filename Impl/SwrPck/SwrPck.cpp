#include "icomp/export.h"

#include "SwrPck.h"


namespace SwrPck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE("SwrPck", "Standard SwissRanger hardware package", "SwissRanger Standard");

I_EXPORT_COMPONENT(
			SwissRangerAcquisition,
			"Implementation of camera interface using SwissRanger API",
			"SwissRanger Camera 3D Acquisition Bitmap Image",
			icomp::IComponentStaticInfo::CCT_DATA_PROCESSING);

I_EXPORT_COMPONENT(
			SwissRangerParams,
			"SwissRanger parameters",
			"SwissRanger Camera 3D Acquisition Parameter",
			icomp::IComponentStaticInfo::CCT_DATA);

I_EXPORT_COMPONENT(
			AttributedSwissRangerParams,
			"Attributed SwissRanger parameters",
			"SwissRanger Camera 3D Acquisition Parameter",
			icomp::IComponentStaticInfo::CCT_DATA);

I_EXPORT_COMPONENT(
			SwissRangerAcquisitionData,
			"SwissRanger acquisition data",
			"SwissRanger Camera 3D Acquisition Parameter Image",
			icomp::IComponentStaticInfo::CCT_DATA);


} // namespace SwrPck


