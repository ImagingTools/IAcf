#include "icomp/export.h"

#include "OpenCvPck.h"


namespace OpenCvPck
{


I_EXPORT_PACKAGE(
			"Iacf/OpenCv",
			"OpenCV integration",
			"OpenCV" IM_PROJECT("IACF") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			OpenCvAcquisition,
			"Implementation of camera interface using OpenCV API",
			"OpenCV Camera Acquisition Bitmap Image" IM_TAG("Camera"));

I_EXPORT_COMPONENT(
			OpenCvVideoController,
			"OpenCV-based video controller",
			"OpenCV Video Multimedia Controller" IM_TAG("Multimedia"));


} // namespace OpenCvPck


