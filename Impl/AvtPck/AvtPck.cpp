#include "icomp/export.h"

#include "AvtPck.h"


namespace AvtPck
{


I_EXPORT_DEFAULT_SERVICES


I_EXPORT_PACKAGE(
			"AvtPck",
			"Standard Applied Vision Technologies (AVT) hardware package based on Qt",
			"AVT Applied Vision Technologies Qt Standard" IM_PROJECT("\"ACF Computer Vision\"") IM_TAG("Qt \"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));


I_EXPORT_COMPONENT(
			FireGrabAcquisition,
			"Implementation of camera interface using FireGrab API",
			"AVT Grabber Camera Acquisition Bitmap Image FireGrab");

I_EXPORT_COMPONENT(
			TriggerParams,
			"Trigger params for cameras",
			"Grabber Camera Acquisition Bitmap Image FireGrab Parameter");


} // namespace AvtPck


