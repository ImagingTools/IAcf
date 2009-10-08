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

	istd::TChangeNotifier<iimg::IBitmap> bitmapPtr(dynamic_cast<iimg::IBitmap*>(outputPtr));
	if (!bitmapPtr.IsValid()){
		return TS_INVALID;
	}

	IplImage* iplImagePtr = cvQueryFrame(m_cameraPtr);
	if (iplImagePtr == NULL){
		return TS_INVALID;
	}


	if (iplImagePtr->depth == IPL_DEPTH_8U){
		switch (iplImagePtr->nChannels){
			case 1:
				if (!bitmapPtr->CreateBitmap(
							istd::CIndex2d(iplImagePtr->width, iplImagePtr->height),
							iplImagePtr->imageData,
							true,
							iplImagePtr->widthStep,
							32,
							4)){
					return TS_INVALID;
				}
				break;
			case 3:
				if (bitmapPtr->CreateBitmap(istd::CIndex2d(iplImagePtr->width, iplImagePtr->height), 32, 4)){
					for (int y = 0; y < iplImagePtr->height; y++){
						I_BYTE* outputBitmapLinePtr = (I_BYTE*)bitmapPtr->GetLinePtr(y);
						I_BYTE* inputBitmapLinePtr = (I_BYTE*)iplImagePtr->imageData + y * iplImagePtr->widthStep;

						for (int x = 0; x < bitmapPtr->GetLineBytesCount(); x += 4){
							int ix = (x >> 2) * 3;
							outputBitmapLinePtr[x + 0] = inputBitmapLinePtr[ix + 2];
							outputBitmapLinePtr[x + 1] = inputBitmapLinePtr[ix + 1];
							outputBitmapLinePtr[x + 2] = inputBitmapLinePtr[ix + 0];
							outputBitmapLinePtr[x + 3] = 255;
						}
					}

					return TS_OK;
				}
				break;
		}
	}

	return TS_INVALID;
}


// reimplemented (icam::IBitmapAcquisition)

istd::CIndex2d COpenCvAcquisitionComp::GetBitmapSize(const iprm::IParamsSet* /*paramsPtr*/) const
{
	int imageWidth = (int)cvGetCaptureProperty(m_cameraPtr, CV_CAP_PROP_FRAME_WIDTH);
	int imageHeight = (int)cvGetCaptureProperty(m_cameraPtr, CV_CAP_PROP_FRAME_HEIGHT);

	return istd::CIndex2d(imageWidth, imageHeight);
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

	m_cameraPtr = cvCreateCameraCapture(CV_CAP_ANY);

	if (m_cameraPtr != NULL){
		cvSetCaptureProperty(m_cameraPtr, CV_CAP_PROP_FRAME_WIDTH, 640);
		cvSetCaptureProperty(m_cameraPtr, CV_CAP_PROP_FRAME_HEIGHT, 480);
	}
}


void COpenCvAcquisitionComp::OnComponentDestroyed()
{
	cvReleaseCapture(&m_cameraPtr);

	BaseClass::OnComponentDestroyed();
}


} // namespace iocv


