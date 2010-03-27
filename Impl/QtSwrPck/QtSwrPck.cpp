#include "icomp/export.h"

#include "QtSwrPck.h"


namespace QtSwrPck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE("QtSwrPck", "Standard SwissRanger hardware package based on Qt", "SwissRanger Qt Standard");

I_EXPORT_COMPONENT(
		SwissRangerParamsGui,
		"SwissRanger parameter editor",
		"SwissRanger Camera 3D Acquisition Parameter GUI Editor",
		icomp::IComponentStaticInfo::CCT_GUI | icomp::IComponentStaticInfo::CCT_DATA_PRESENTATION);

I_EXPORT_COMPONENT(
			SwissRangerAcquisitionDataView,
			"SwissRanger image view",
			"SwissRanger Camera 3D Acquisition Parameter GUI View Image",
			icomp::IComponentStaticInfo::CCT_GUI | icomp::IComponentStaticInfo::CCT_DATA_PRESENTATION);


} // namespace QtSwrPck


