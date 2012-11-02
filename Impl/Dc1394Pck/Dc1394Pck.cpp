#include "Dc1394Pck.h"


#include "icomp/export.h"


namespace Dc1394Pck
{


I_EXPORT_PACKAGE(
			"Iacf/Dc1394",
			"Package based on dc1394 library",
			"IACF 1394 FireWire" IM_PROJECT("IACF") IM_COMPANY("ImagingTools"));

I_EXPORT_COMPONENT(
			SimpleCamera,
			"Simple camera implementation using FireWire communication",
			"Camera Image Bitmap Acquisition Snap Grabber" IM_TAG("Camera") IM_AUTHOR("\"Witold Gantzke\""));


} // namespace Dc1394Pck


