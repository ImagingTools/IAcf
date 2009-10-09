#include "iocv/COcvVideoControllerComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


// IACF includes
#include "iocv/COcvImage.h"


namespace iocv
{


// public methods

COcvVideoControllerComp::COcvVideoControllerComp()
{
}


// reimplemented (icomp::IComponent)


void COcvVideoControllerComp::OnComponentDestroyed()
{
	CloseMedium();

	BaseClass::OnComponentDestroyed();
}


// reimplemented (imm::IMediaController)

istd::CString COcvVideoControllerComp::GetOpenedMediumUrl() const
{
	return m_mediumUrl;
}


bool COcvVideoControllerComp::OpenMediumUrl(const istd::CString& url, bool autoPlay)
{
	istd::CChangeNotifier notifier(this, CF_STATUS);

	EnsureMediumClosed();

	m_capturePtr.SetPtr(cvCreateFileCapture(url.ToString().c_str()));
	if (!m_capturePtr.IsValid()){
		return false;
	}

	SetPlaying(autoPlay);

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
	return false;
}


bool COcvVideoControllerComp::SetPlaying(bool/* state*/)
{
	istd::CChangeNotifier notifier(this, CF_STATUS);

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
	return GetCurrentFrame() / *m_framesPerSecondAttrPtr;
}


bool COcvVideoControllerComp::SetCurrentPosition(double position)
{
	return SetCurrentFrame(int(position * *m_framesPerSecondAttrPtr + 0.5));
}


// reimplemented (imm::IVideoInfo)

int COcvVideoControllerComp::GetFramesCount() const
{
	if (m_capturePtr.IsValid()){
		return (int)cvGetCaptureProperty(m_capturePtr.GetPtr(), CV_CAP_PROP_FRAME_COUNT);
	}

	return 0;
}


double COcvVideoControllerComp::GetFrameIntervall() const
{
	return 1.0 / *m_framesPerSecondAttrPtr;
}


istd::CIndex2d COcvVideoControllerComp::GetFrameSize() const
{
	return istd::CIndex2d::GetInvalid();
}


double COcvVideoControllerComp::GetPixelAspectRatio() const
{
	return 1;
}


// reimplemented (imm::IVideoController)

int COcvVideoControllerComp::GetCurrentFrame() const
{
	if (m_capturePtr.IsValid()){
		return (int)cvGetCaptureProperty(m_capturePtr.GetPtr(), CV_CAP_PROP_POS_FRAMES);
	}

	return 0;
}


bool COcvVideoControllerComp::SetCurrentFrame(int frameIndex)
{
	if (m_capturePtr.IsValid()){
		bool retVal = SeekToPostion(frameIndex);
		if (retVal){
			iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(m_frameDataCompPtr.GetPtr());
			if (bitmapPtr != NULL){
				return GrabCurrentFrame(*bitmapPtr);
			}
		}
	}

	return false;
}


bool COcvVideoControllerComp::GrabFrame(iimg::IBitmap& result, int frameIndex) const
{
	if (!m_capturePtr.IsValid()){
		return false;
	}

	int currentIndex = GetCurrentFrame();
	bool retVal = false;

	// seek to frame:
	if (SeekToPostion(frameIndex)){
		retVal = GrabCurrentFrame(result);
	}

	// restore original frame:
	return SeekToPostion(currentIndex) && retVal;
}


// protected methods

bool COcvVideoControllerComp::GrabCurrentFrame(iimg::IBitmap& result) const
{
	if (m_capturePtr.IsValid()){
		IplImage* framePtr = cvQueryFrame(m_capturePtr.GetPtr());
		if (framePtr != NULL){
			istd::CChangeNotifier changePtr(&result);

			return iocv::COcvImage::ConvertToBitmap(*framePtr, result);
		}
	}

	return false;
}


bool COcvVideoControllerComp::SeekToPostion(int frameIndex) const
{
	if (m_capturePtr.IsValid()){
		bool retVal = (cvSetCaptureProperty(m_capturePtr.GetPtr(), CV_CAP_PROP_POS_FRAMES, frameIndex) != 0);
		if (retVal){
			int setIndex = GetCurrentFrame();

			return (setIndex == frameIndex);
		}
	}

	return false;
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


} // namespace iocv


