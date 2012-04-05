#include "icomp/export.h"

#include "AvtPck.h"


namespace AvtPck
{


I_EXPORT_PACKAGE(
			"Iacf/Camera/Avt",
			"Standard Applied Vision Technologies (AVT) hardware package based on Qt",
			"AVT Applied Vision Technologies Qt Standard" IM_PROJECT("IACF") IM_TAG("Qt \"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));


I_EXPORT_COMPONENT(
			FireGrabAcquisition,
			"Implementation of camera interface using FireGrab API",
			"AVT Grabber Camera Acquisition Bitmap Image FireGrab");


} // namespace AvtPck


