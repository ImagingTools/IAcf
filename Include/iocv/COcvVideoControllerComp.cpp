#include <iocv/COcvVideoControllerComp.h>


// Qt includes
#include <QtCore/QStringList>


// ACF includes
#include <istd/CChangeNotifier.h>


// IACF includes
#include <iocv/COcvImage.h>


namespace iocv
{


const istd::IChangeable::ChangeSet s_openMediaChangeSet(imm::IMediaController::CF_STATUS, imm::IMediaController::CF_MEDIA_POSITION);
const istd::IChangeable::ChangeSet s_closeMediaChangeSet(imm::IMediaController::CF_STATUS);
const istd::IChangeable::ChangeSet s_startStopChangeSet(imm::IMediaController::CF_STATUS);
const istd::IChangeable::ChangeSet s_setPositionChangeSet(imm::IMediaController::CF_MEDIA_POSITION);


// public methods

COcvVideoControllerComp::COcvVideoControllerComp()
:	m_isPlaying(false),
	m_currentFrameIndex(0)
{
}


// reimplemented (icam::IBitmapAcquisition)

istd::CIndex2d COcvVideoControllerComp::GetBitmapSize(const iprm::IParamsSet* /*paramsPtr*/) const
{
	return GetFrameSize();
}


// reimplemented (iproc::IProcessor)

int COcvVideoControllerComp::DoProcessing(
			const iprm::IParamsSet* /*paramsPtr*/,
			const istd::IPolymorphic* /*inputPtr*/,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* /*progressManagerPtr*/)
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

QString COcvVideoControllerComp::GetOpenedMediumUrl() const
{
	return m_mediumUrl;
}


bool COcvVideoControllerComp::OpenMediumUrl(const QString& url, bool autoPlay)
{
	if (m_mediumUrl != url){
		EnsureMediumClosed();

		m_capturePtr.SetPtr(new cv::VideoCapture(url.toStdString()));
		if (!m_capturePtr.IsValid()){
			return false;
		}

		m_framesCount = (int)m_capturePtr->get(CV_CAP_PROP_FRAME_COUNT);

		istd::CChangeNotifier notifier(this, &s_openMediaChangeSet);
		Q_UNUSED(notifier);

		m_mediumUrl = url;

		SetPlaying(autoPlay);
	}

	return true;
}


void COcvVideoControllerComp::CloseMedium()
{
	EnsureMediumClosed();

	istd::CChangeNotifier notifier(this, &s_closeMediaChangeSet);
	Q_UNUSED(notifier);

	SetPlaying(false);
}


bool COcvVideoControllerComp::IsPlaying() const
{
	return m_isPlaying;
}


bool COcvVideoControllerComp::SetPlaying(bool state)
{
	if (m_isPlaying != state){
		istd::CChangeNotifier notifier(this, &s_startStopChangeSet);
		Q_UNUSED(notifier);

		m_isPlaying = state;
	}

	return true;
}


double COcvVideoControllerComp::GetMediumLength() const
{
	double fps = 0;
	if (m_capturePtr.IsValid()){
		fps = m_capturePtr->get(CV_CAP_PROP_FPS);
	}

	return fps * GetFramesCount();
}


double COcvVideoControllerComp::GetCurrentPosition() const
{
	return GetCurrentFrame() * GetFrameInterval();
}


bool COcvVideoControllerComp::SetCurrentPosition(double position)
{
	return SetCurrentFrame(int(position / GetFrameInterval() + 0.5));
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


double COcvVideoControllerComp::GetFrameInterval() const
{
	if (m_capturePtr.IsValid()){
		return 1.0 / m_capturePtr->get(CV_CAP_PROP_FPS);
	}

	return 0;
}


istd::CIndex2d COcvVideoControllerComp::GetFrameSize() const
{
	if (m_capturePtr.IsValid()){
		int frameWidth = (int)m_capturePtr->get(CV_CAP_PROP_FRAME_WIDTH);
		int frameHeight = (int)m_capturePtr->get(CV_CAP_PROP_FRAME_HEIGHT);

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
			istd::CChangeNotifier notifier(this, &s_setPositionChangeSet);
			Q_UNUSED(notifier);

			iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(m_frameDataCompPtr.GetPtr());
			if (bitmapPtr != NULL){
				return GrabCurrentFrame(*bitmapPtr);
			}
		}
	}

	return false;
}


// reimplemented (ifile::IFileTypeInfo)

bool COcvVideoControllerComp::GetFileExtensions(QStringList& result, const istd::IChangeable* /*dataObjectPtr*/, int flags, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	if ((flags & QF_LOAD) != 0){
		result.push_back("mpg");
		result.push_back("mpeg");
		result.push_back("mov");
		result.push_back("wmv");
		result.push_back("avi");
		result.push_back("m2p");
		result.push_back("mp4");
	}

	return true;
}


QString COcvVideoControllerComp::GetTypeDescription(const QString* /*extensionPtr*/) const
{
	return "Video files";
}


// protected methods

bool COcvVideoControllerComp::GrabCurrentFrame(iimg::IBitmap& result) const
{
	if (m_capturePtr.IsValid()){
		cv::Mat frame;
		if (m_capturePtr->retrieve(frame)){
			istd::CChangeNotifier changePtr(&result);

			return iocv::COcvImage::ConvertToBitmap(frame, result);
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
			if (m_capturePtr->set(CV_CAP_PROP_POS_FRAMES, frameIndex) != 0){
				if (m_capturePtr->grab()){
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
	m_capturePtr.Reset();
}


int COcvVideoControllerComp::GetStreamPosition() const
{
	if (m_capturePtr.GetPtr()){
		return (int)m_capturePtr->get(CV_CAP_PROP_POS_FRAMES);
	}

	return 0;
}


bool COcvVideoControllerComp::MoveToNextFrame() const
{
	if (m_capturePtr.IsValid()){
		if (m_capturePtr->grab()){
			m_currentFrameIndex++;

			return true;
		}
	}

	return false;
}


} // namespace iocv


