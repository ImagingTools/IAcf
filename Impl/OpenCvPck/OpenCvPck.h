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



} // namespace OpenCvPck


#endif // !OpenCvPck_included


