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

I_EXPORT_COMPONENT(
			OpenCvBlobExtractor,
			"OpenCV-based blob extraction",
			"OpenCV Blob Search Object");

I_EXPORT_COMPONENT(
			CorrSearchProcessor,
			"OpenCV-based normalized correlation search processor",
			"OpenCV Normalized Correlation Search Processor Model Image Find Pattern Matching");


} // namespace OpenCvPck


