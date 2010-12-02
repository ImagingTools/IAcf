#include "icomp/export.h"

#include "IacfBasePck.h"


namespace IacfBasePck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE(
			"IacfBase",
			"General, system-independent image processing package",
			"Standard Base Imaging Tools" IM_PROJECT("IACF") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));


I_EXPORT_COMPONENT(
			ExposureParams,
			"Parameters for exposure parameter control",
			"Exposure Shutter Delay Time EEN Image Bitmap Acquisition Snap Parameters");

I_EXPORT_COMPONENT(
			TriggerParams,
			"Trigger params for cameras",
			"Camera Acquisition Trigger Parameters Snap");

I_EXPORT_COMPONENT(
			SnapBitmapSupplier,
			"Implementation of bitmap supplier getting image from camera",
			"Image Bitmap Acquisition Snap Supplier Parameters");

I_EXPORT_COMPONENT(
			MemoryAcquisition,
			"Image acquisition from the image itself",
			"Image Bitmap Camera Snap Acquisition");

I_EXPORT_COMPONENT(
			AcquisitionConstraints,
			"Static acquisition constraints for some device",
			"Image Bitmap Camera Snap Acquisition Constraints");

I_EXPORT_COMPONENT(
			SamplingParams,
			"Sampling params based on fixed boundary values",
			"Sample Sampling Acquisition Model Parameters");

I_EXPORT_COMPONENT(
			InspectionTask,
			"Group set of suppliers into single inspection task",
			"Task Model Supplier Composite" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Inspection Model"));


} // namespace IacfBasePck


