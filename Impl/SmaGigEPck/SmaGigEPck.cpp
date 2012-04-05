#include "icomp/export.h"

#include "SmaGigEPck.h"


namespace SmaGigEPck
{


I_EXPORT_PACKAGE(
			"Iacf/Camera/GigE/Smartek",
			"GigE Vision camera package based on Smartek SDK",
			"GigE Vision Qt Standard" IM_PROJECT("IACF") IM_TAG("Qt \"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\""));


I_EXPORT_COMPONENT(
			GenicamCamera,
			"Implementation of camera interface using Genicam interface",
			"Genicam Grabber Camera Acquisition Bitmap Image");


} // namespace SmaGigEPck


