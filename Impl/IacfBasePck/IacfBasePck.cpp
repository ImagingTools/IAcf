#include "icomp/export.h"

#include "IacfBasePck.h"


namespace IacfBasePck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE("IacfBase", "ImagingTools system-undependent general package", "Undependend Standard Base Imaging Tools");

I_EXPORT_COMPONENT(
			ExposureParams,
			"Parameters for exposure parameter control",
			"Exposure Shutter Delay Time EEN Image Bitmap Acquisition Snap Parameters",
			icomp::IComponentStaticInfo::CCT_DATA);

I_EXPORT_COMPONENT(
			SnapBitmapSupplier,
			"Implementation of bitmap supplier getting image from camera",
			"Image Bitmap Acquisition Snap Supplier Parameters",
			icomp::IComponentStaticInfo::CCT_DATA_PROCESSING);

I_EXPORT_COMPONENT(
			MemoryAcquisition,
			"Image acquisition from the image itself",
			"Image Bitmap Camera Snap Acquisition",
			icomp::IComponentStaticInfo::CCT_DATA_PROCESSING);

I_EXPORT_COMPONENT(
			AcquisitionConstraints,
			"Static acquisition constraints for some device",
			"Image Bitmap Camera Snap Acquisition Constraints",
			icomp::IComponentStaticInfo::CCT_DATA);


} // namespace IacfBasePck


