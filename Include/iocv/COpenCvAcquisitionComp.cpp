#include "iocv/COpenCvAcquisitionComp.h"


#include "istd/TChangeNotifier.h"

#include "iprm/IParamsSet.h"


namespace iocv
{


COpenCvAcquisitionComp::COpenCvAcquisitionComp()
:	m_cameraPtr(NULL)
{
}


bool COpenCvAcquisitionComp::IsCameraValid() const
{
	return (m_cameraPtr != NULL);
}


// reimplemented (iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>)

int COpenCvAcquisitionComp::DoProcessing(
			const iprm::IParamsSet* /* paramsPtr*/,
			const istd::IPolymorphic* /*inputPtr*/,
			istd::IChangeable* outputPtr)
{
	if (m_cameraPtr == NULL){
		return TS_INVALID;
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (outputBitmapPtr == NULL){
		return TS_INVALID;
	}

	IplImage* iplImagePtr = cvQueryFrame(m_cameraPtr);
	if (iplImagePtr == NULL){
		return TS_INVALID;
	}

	if (outputBitmapPtr->CreateBitmap(
					istd::CIndex2d(iplImagePtr->width, iplImagePtr->height),
					iplImagePtr->imageData,
					true,
					iplImagePtr->widthStep,
					iplImagePtr->depth,
					iplImagePtr->nChannels)){
		return TS_OK;
	}

	return TS_INVALID;
}


// reimplemented (icam::IBitmapAcquisition)

istd::CIndex2d COpenCvAcquisitionComp::GetBitmapSize(const iprm::IParamsSet* /*paramsPtr*/) const
{

	return istd::CIndex2d(-1, -1);	// unknown size
}


// reimplemented (icam::ITriggerConstraints)

bool COpenCvAcquisitionComp::IsTriggerSupported() const
{
	return false;
}


bool COpenCvAcquisitionComp::IsTriggerModeSupported(int /*triggerMode*/) const
{
	return false;
}


// reimplemented (icomp::IComponent)

void COpenCvAcquisitionComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_cameraPtr = cvCreateCameraCapture(0);
}


void COpenCvAcquisitionComp::OnComponentDestroyed()
{
	cvReleaseCapture(&m_cameraPtr);

	BaseClass::OnComponentDestroyed();
}


} // namespace iocv


