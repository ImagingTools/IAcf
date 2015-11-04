#ifndef OpenCvPck_included
#define OpenCvPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

// IACF includes
#include "iocv/COcvAcquisitionComp.h"
#include "iocv/COcvVideoControllerComp.h"
#include "iocv/COcvBlobProcessorComp.h"
#include "iocv/CCorrSearchProcessorComp.h"


/**
	OpenCV-specific package.
*/
namespace OpenCvPck
{


typedef iocv::COcvAcquisitionComp OpenCvAcquisition;
typedef icomp::TModelCompWrap<iocv::COcvVideoControllerComp> OpenCvVideoController;
typedef iocv::COcvBlobProcessorComp OpenCvBlobExtractor;
typedef iocv::CCorrSearchProcessorComp CorrSearchProcessor;


} // namespace OpenCvPck


#endif // !OpenCvPck_included


