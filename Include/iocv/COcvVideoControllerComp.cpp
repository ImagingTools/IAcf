#include "iocv/COcvVideoControllerComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


// IACF includes
#include "iocv/COcvImage.h"


namespace iocv
{


// public methods

COcvVideoControllerComp::COcvVideoControllerComp()
:	m_isPlaying(false),
	m_currentFrameIndex(0)
{
}


// reimplemented (iproc::IBitmapAcquisition)

istd::CIndex2d COcvVideoControllerComp::GetBitmapSize(const iprm::IParamsSet* /*paramsPtr*/) const
{
	return GetFrameSize();
}


// reimplemented (iproc::IProcessor)

int COcvVideoControllerComp::DoProcessing(
			const iprm::IParamsSet* /*paramsPtr*/,
			const istd::IPolymorphic* /*inputPtr*/,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* /*progressManagerPtr*/)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if ((bitmapPtr != NULL) && GrabCurrentFrame(*bitmapPtr)){
		return TS_OK;
	}

	return TS_INVALID;
}


// reimplemented (imm::IMediaController)

istd::CString COcvVideoControllerComp::GetOpenedMediumUrl() const
{
	return m_mediumUrl;
}


bool COcvVideoControllerComp::OpenMediumUrl(const istd::CString& url, bool autoPlay)
{
	if (m_mediumUrl != url){
		EnsureMediumClosed();

		m_capturePtr.SetPtr(cvCreateFileCapture(url.ToString().c_str()));
		if (!m_capturePtr.IsValid()){
			return false;
		}

		cvQueryFrame(m_capturePtr.GetPtr());

		m_framesCount = (int)cvGetCaptureProperty(m_capturePtr.GetPtr(), CV_CAP_PROP_FRAME_COUNT);

		istd::CChangeNotifier notifier(this, CF_STATUS);

		m_mediumUrl = url;

		SetPlaying(autoPlay);
	}

	return true;
}


void COcvVideoControllerComp::CloseMedium()
{
	EnsureMediumClosed();

	istd::CChangeNotifier notifier(this, CF_STATUS);

	SetPlaying(false);
}


bool COcvVideoControllerComp::IsPlaying() const
{
	return m_isPlaying;
}


bool COcvVideoControllerComp::SetPlaying(bool state)
{
	if (m_isPlaying != state){
		istd::CChangeNotifier notifier(this, CF_STATUS);

		m_isPlaying = state;
	}

	return true;
}


double COcvVideoControllerComp::GetMediumLength() const
{
	double fps = 0;
	if (m_capturePtr.IsValid()){
		fps = cvGetCaptureProperty(m_capturePtr.GetPtr(), CV_CAP_PROP_FPS);
	}

	return fps * GetFramesCount();
}


double COcvVideoControllerComp::GetCurrentPosition() const
{
	return GetCurrentFrame() * GetFrameIntervall();
}


bool COcvVideoControllerComp::SetCurrentPosition(double position)
{
	return SetCurrentFrame(int(position / GetFrameIntervall() + 0.5));
}


int COcvVideoControllerComp::GetSupportedFeatures() const
{
	return SF_OPEN_MEDIA | SF_SEEK;
}


// reimplemented (imm::IVideoInfo)

int COcvVideoControllerComp::GetFramesCount() const
{
	return m_framesCount;
}


double COcvVideoControllerComp::GetFrameIntervall() const
{
	if (m_capturePtr.IsValid()){
		return 1.0 / cvGetCaptureProperty(m_capturePtr.GetPtr(), CV_CAP_PROP_FPS);
	}

	return 0;
}


istd::CIndex2d COcvVideoControllerComp::GetFrameSize() const
{
	if (m_capturePtr.IsValid()){
		int frameWidth = (int)cvGetCaptureProperty(m_capturePtr.GetPtr(), CV_CAP_PROP_FRAME_WIDTH);
		int frameHeight = (int)cvGetCaptureProperty(m_capturePtr.GetPtr(), CV_CAP_PROP_FRAME_HEIGHT);

		return istd::CIndex2d(frameWidth, frameHeight);
	}
	
	return istd::CIndex2d::GetInvalid();
}


double COcvVideoControllerComp::GetPixelAspectRatio() const
{
	return -1;
}


// reimplemented (imm::IVideoController)

int COcvVideoControllerComp::GetCurrentFrame() const
{
	return m_currentFrameIndex;
}


bool COcvVideoControllerComp::SetCurrentFrame(int frameIndex)
{
	if (m_capturePtr.IsValid()){
		bool retVal = SeekToPosition(frameIndex);
		if (retVal){
			istd::CChangeNotifier notifier(this, CF_MEDIA_POSITION);

			iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(m_frameDataCompPtr.GetPtr());
			if (bitmapPtr != NULL){
				return GrabCurrentFrame(*bitmapPtr);
			}
		}
	}

	return false;
}


// protected methods

bool COcvVideoControllerComp::GrabCurrentFrame(iimg::IBitmap& result) const
{
	if (m_capturePtr.IsValid()){
		IplImage* framePtr = cvRetrieveFrame(m_capturePtr.GetPtr());
		if (framePtr != NULL){
			istd::CChangeNotifier changePtr(&result);

			return iocv::COcvImage::ConvertToBitmap(*framePtr, result);
		}
	}

	return false;
}


bool COcvVideoControllerComp::SeekToPosition(int frameIndex) const
{
	if (m_currentFrameIndex == frameIndex){
		return true;
	}

	int posDiff = frameIndex - m_currentFrameIndex;

	bool retVal = true;

	if (posDiff > 0){
		while (posDiff--){
			retVal = retVal && MoveToNextFrame();
		}
	}
	else{
		if (m_capturePtr.GetPtr()){
			if (cvSetCaptureProperty(m_capturePtr.GetPtr(), CV_CAP_PROP_POS_FRAMES, frameIndex) != 0){
				if (cvGrabFrame(m_capturePtr.GetPtr()) != 0){
					m_currentFrameIndex = frameIndex;
				}
			}
		}
	}

	return retVal;
}


// reimplemented (icomp::CComponentBase)

void COcvVideoControllerComp::OnComponentDestroyed()
{
	CloseMedium();

	BaseClass::OnComponentDestroyed();
}


// private methods

void COcvVideoControllerComp::EnsureMediumClosed()
{
	CvCapture* capturePtr = m_capturePtr.GetPtr();
	if (capturePtr != NULL){
		cvReleaseCapture(&capturePtr);
	}

	m_capturePtr.Reset();
}


int COcvVideoControllerComp::GetStreamPosition() const
{
	if (m_capturePtr.GetPtr()){
		return (int)cvGetCaptureProperty(m_capturePtr.GetPtr(), CV_CAP_PROP_POS_FRAMES);
	}

	return 0;
}


bool COcvVideoControllerComp::MoveToNextFrame() const
{
	if (m_capturePtr.IsValid()){
		if (cvGrabFrame(m_capturePtr.GetPtr()) != 0){
			m_currentFrameIndex++;

			return true;
		}
	}

	return false;
}


} // namespace iocv


