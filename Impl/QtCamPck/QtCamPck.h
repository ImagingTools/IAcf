#ifndef QtCamPck_included
#define QtCamPck_included


#include "iqtcam/CFileAcquisitionComp.h"
#include "iqtcam/CSnapImageGuiComp.h"
#include "iqtcam/CExposureParamsGuiComp.h"
#include "iqtcam/CSnapBitmapSupplierGuiComp.h"
#include "iqtcam/CMemoryAcquisitionComp.h"


/**
	Qt camera package.
*/
namespace QtCamPck
{


typedef iqtcam::CFileAcquisitionComp FileAcquisition;
typedef iqtcam::CSnapImageGuiComp SnapImageGui;
typedef iqtcam::CExposureParamsGuiComp ExposureParamsGui;
typedef iqtcam::CSnapBitmapSupplierGuiComp SnapBitmapSupplierGui;
typedef iqtcam::CMemoryAcquisitionComp MemoryAcquisition;


} // namespace QtCamPck


#endif // !QtCamPck_included


