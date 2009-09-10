#include "icomp/export.h"

#include "IacfBasePck.h"


namespace IacfBasePck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE("IacfBase", "ImagingTools system-undependent general package", "Undependend Standard Base Imaging Tools");

I_EXPORT_COMPONENT(TestIdManager, "Implementation of ID manager generating ID's for test", "ID Manager Test Supplier");
I_EXPORT_COMPONENT(CascadedProcessor, "Sequence processing using slave processors", "Processor Sequence Buffer Cascade");
I_EXPORT_COMPONENT(ExposureParams, "Parameters for exposure parameter control", "Exposure Shutter Delay Time EEN Image Bitmap Acquisition Snap Parameters");
I_EXPORT_COMPONENT(SnapBitmapSupplier, "Implementation of bitmap supplier getting image from camera", "Image Bitmap Acquisition Snap Supplier Parameters");
I_EXPORT_COMPONENT(MemoryAcquisition, "Image acquisition from the image itself", "Image Bitmap Camera Snap Acquisition");
I_EXPORT_COMPONENT(AcquisitionConstraints, "Static acquisition constraints for some device", "Image Bitmap Camera Snap Acquisition Constraints");


} // namespace IacfBasePck


