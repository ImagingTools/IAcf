#include "icomp/export.h"

#include "OpenCvPck.h"


namespace OpenCvPck
{

I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE("OpenCvPck", "Standard OpenCV package", "OpenCV Standard Image Processing Camera");

I_EXPORT_COMPONENT(OpenCvAcquisition, "Implementation of camera interface using OpenCV API", "OpenCV Camera Acquisition Bitmap Image");


} // namespace OpenCvPck


