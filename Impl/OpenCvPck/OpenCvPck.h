#ifndef OpenCvPck_included
#define OpenCvPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"


// IACF includes
#include "iocv/COpenCvAcquisitionComp.h"
#include "iocv/COcvVideoControllerComp.h"


/**
	OpenCV-specific package.
*/
namespace OpenCvPck
{


typedef iocv::COpenCvAcquisitionComp OpenCvAcquisition;
typedef icomp::TModelCompWrap<iocv::COcvVideoControllerComp> OpenCvVideoController;

} // namespace OpenCvPck


#endif // !OpenCvPck_included


