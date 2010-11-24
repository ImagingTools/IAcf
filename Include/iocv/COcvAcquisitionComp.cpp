#include "iocv/COcvAcquisitionComp.h"


// ACF includes 
#include "istd/TChangeNotifier.h"

#include "iprm/IParamsSet.h"


// IACF includes
#include "iocv/COcvImage.h"


namespace iocv
{


COcvAcquisitionComp::COcvAcquisitionComp()
:	m_cameraPtr(NULL)
{
}


bool COcvAcquisitionComp::IsCameraValid() const
{
	return (m_cameraPtr != NULL);
}


// reimplemented (iproc::TSyncProcessorWrap<iproc::IBitmapAcquisition>)

int COcvAcquisitionComp::DoProcessing(
			const iprm::IParamsSet* /* paramsPtr*/,
			const istd::IPolymorphic* /*inputPtr*/,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* /*progressManagerPtr*/)
{
	if (m_cameraPtr == NULL){
		return TS_INVALID;
	}

	istd::TChangeNotifier<iimg::IBitmap> bitmapPtr(dynamic_cast<iimg::IBitmap*>(outputPtr));
	if (!bitmapPtr.IsValid()){
		return TS_INVALID;
	}

	IplImage* iplImagePtr = cvQueryFrame(m_cameraPtr);
	if (iplImagePtr == NULL){
		return TS_INVALID;
	}

	if (iocv::COcvImage::ConvertToBitmap(*iplImagePtr, *bitmapPtr.GetPtr())){
		return TS_OK;
	}

	return TS_INVALID;
}


// reimplemented (iproc::IBitmapAcquisition)

istd::CIndex2d COcvAcquisitionComp::GetBitmapSize(const iprm::IParamsSet* /*paramsPtr*/) const
{
	int imageWidth = (int)cvGetCaptureProperty(m_cameraPtr, CV_CAP_PROP_FRAME_WIDTH);
	int imageHeight = (int)cvGetCaptureProperty(m_cameraPtr, CV_CAP_PROP_FRAME_HEIGHT);

	return istd::CIndex2d(imageWidth, imageHeight);
}


// reimplemented (icam::ITriggerConstraints)

bool COcvAcquisitionComp::IsTriggerSupported() const
{
	return false;
}


bool COcvAcquisitionComp::IsTriggerModeSupported(int /*triggerMode*/) const
{
	return false;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void COcvAcquisitionComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_cameraPtr = cvCreateCameraCapture(CV_CAP_ANY);
	if (m_cameraPtr != NULL){
		cvSetCaptureProperty(m_cameraPtr, CV_CAP_PROP_FRAME_WIDTH, 640);
		cvSetCaptureProperty(m_cameraPtr, CV_CAP_PROP_FRAME_HEIGHT, 480);
	}
}


void COcvAcquisitionComp::OnComponentDestroyed()
{
	if (m_cameraPtr != NULL){
		cvReleaseCapture(&m_cameraPtr);
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace iocv


