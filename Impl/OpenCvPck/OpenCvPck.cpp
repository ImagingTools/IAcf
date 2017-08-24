#include "OpenCvPck.h"


#include <icomp/export.h>


namespace OpenCvPck
{


I_EXPORT_PACKAGE(
			"Iacf/OpenCv",
			"OpenCV integration",
			"OpenCV" IM_PROJECT("IACF") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			OpenCvAcquisition,
			"Implementation of camera interface using OpenCV API",
			"Camera Acquisition Bitmap Image" IM_TAG("Camera"));

I_EXPORT_COMPONENT(
			OpenCvVideoController,
			"OpenCV-based video controller",
			"Video Multimedia Controller" IM_TAG("Multimedia"));

I_EXPORT_COMPONENT(
			OpenCvBlobExtractor,
			"OpenCV-based blob extraction",
			"Blob Search Object");

I_EXPORT_COMPONENT(
			CorrSearchProcessor,
			"OpenCV-based normalized correlation search processor",
			"Normalized Correlation Search Processor Model Image Find Pattern Matching");

I_EXPORT_COMPONENT(
			OpenCvMedianProcessor,
			"OpenCV-based median blur processor",
			"Median Blue Smoothing Processor");

I_EXPORT_COMPONENT(
			HoughBasedSearchProcessor,
			"OpenCV-based search processor based on hough transformation",
			"Search Circle Line Processor Feature Hough");


} // namespace OpenCvPck


