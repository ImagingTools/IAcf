#include "icomp/export.h"

#include "QtSwrPck.h"


namespace QtSwrPck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE(
			"QtSwrPck",
			"Standard SwissRanger integration package based on Qt",
			"SwissRanger TOF Qt \"Image Processing\"" IM_PROJECT("IACF") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
		SwissRangerParamsGui,
		"SwissRanger parameter editor",
		"SwissRanger Camera 3D Acquisition Parameter GUI Editor");

I_EXPORT_COMPONENT(
			SwissRangerAcquisitionDataView,
			"SwissRanger image view",
			"SwissRanger Camera 3D Acquisition Parameter GUI View Image");


} // namespace QtSwrPck


