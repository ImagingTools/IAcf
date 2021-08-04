#ifndef OpenCvPck_included
#define OpenCvPck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

// IACF includes
#include <iocv/COcvAcquisitionComp.h>
#include <iocv/COcvVideoControllerComp.h>
#include <iocv/COcvBlobProcessorComp.h>
#include <iocv/CCorrSearchProcessorComp.h>
#include <iocv/COcvMedianProcessorComp.h>
#include <iocv/CHoughBasedSearchProcessorComp.h>
#include <iocv/COcvResamplingProcessorComp.h>
#include <iocv/COcvEdgeExtractorComp.h>
#include <iocv/COcvAdaptiveBinarizationComp.h>
#include <iocv/COcvMorphologicalProcessorComp.h>
#include <iocv/COcvPointGridExtractorComp.h>
#include <iocv/COcvIntrinsicCameraCalibration.h>
#include <iocv/COcvIntrinsicCameraCalibrationSupplierComp.h>
#include <iocv/COcvUndistortImageProcessorComp.h>
#include <iocv/COcvDenoisingProcessorComp.h>


/**
	OpenCV-specific package.
*/
namespace OpenCvPck
{


typedef iocv::COcvAcquisitionComp OpenCvAcquisition;
typedef icomp::TModelCompWrap<iocv::COcvVideoControllerComp> OpenCvVideoController;
typedef iocv::COcvBlobProcessorComp OpenCvBlobExtractor;
typedef iocv::CCorrSearchProcessorComp OpenCvCorrSearchProcessor;
typedef iocv::COcvMedianProcessorComp OpenCvMedianProcessor;
typedef iocv::CHoughBasedSearchProcessorComp OpenCvHoughBasedSearchProcessor;
typedef iocv::COcvResamplingProcessorComp OpenCvResamplingProcessor;
typedef iocv::COcvEdgeExtractorComp OpenCvEdgeExtractor;
typedef iocv::COcvAdaptiveBinarizationComp OpenCvAdaptiveBinarization;
typedef iocv::COcvMorphologicalProcessorComp OpenCvMorphologicalProcessor;
typedef iocv::COcvPointGridExtractorComp OpenCvPointGridExtractor;
typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						iocv::COcvIntrinsicCameraCalibration,
						i2d::ICalibration2d,
						iser::ISerializable>> OpenCvIntrinsicCameraCalibration;
typedef icomp::TModelCompWrap<iocv::COcvIntrinsicCameraCalibrationSupplierComp> OpenCvIntrinsicCameraCalibrationSupplier;
typedef iocv::COcvUndistortImageProcessorComp OpenCvUndistortImageProcessor;
typedef iocv::COcvDenoisingProcessorComp OpenCvDenoisingProcessor;


} // namespace OpenCvPck


#endif // !OpenCvPck_included


