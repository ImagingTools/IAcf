#include "icomp/export.h"

#include "SwrPck.h"


namespace SwrPck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE("SwrPck", "Standard SwissRanger hardware package", "SwissRanger Standard");

I_EXPORT_COMPONENT(SwissRangerAcquisition, "Implementation of camera interface using SwissRanger API", "SwissRanger Camera 3D Acquisition Bitmap Image");


} // namespace SwrPck


