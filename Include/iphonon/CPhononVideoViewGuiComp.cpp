#include "iphonon/CPhononVideoViewGuiComp.h"


// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QEventLoop>
#include <QtCore/QUrl>

// ACF includes
#include "istd/CChangeNotifier.h"


namespace iphonon
{


// public methods

CPhononVideoViewGuiComp::CPhononVideoViewGuiComp()
:	m_currentPosition(0)
{
}


// reimplemented (imm::IMediaController)

QString CPhononVideoViewGuiComp::GetOpenedMediumUrl() const
{
	EnsureSync();

	return m_mediaPlayer.currentMedia().canonicalUrl().path();
}


bool CPhononVideoViewGuiComp::OpenMediumUrl(const QString& url, bool autoPlay)
{
	static const ChangeSet changeSet(CF_STATUS, CF_MEDIA_POSITION);
	istd::CChangeNotifier notifier(this, &changeSet);
	Q_UNUSED(notifier);

	m_mediaPlayer.setMedia(QUrl(url));

	if (autoPlay){
		m_mediaPlayer.play();
	}
	else{
		m_mediaPlayer.pause();
	}

	return true;
}


void CPhononVideoViewGuiComp::CloseMedium()
{
	static const ChangeSet changeSet(CF_STATUS);
	istd::CChangeNotifier notifier(this, &changeSet);
	Q_UNUSED(notifier);

	m_mediaPlayer.stop();
}


bool CPhononVideoViewGuiComp::IsPlaying() const
{
	EnsureSync();

	QMediaPlayer::State state = m_mediaPlayer.state();
	QMediaPlayer::MediaStatus mediaStatus = m_mediaPlayer.mediaStatus();

	return (state == QMediaPlayer::PlayingState) || (mediaStatus == QMediaPlayer::BufferingMedia);
}


bool CPhononVideoViewGuiComp::SetPlaying(bool state)
{
	static const ChangeSet changeSet(CF_STATUS);
	istd::CChangeNotifier notifier(this, &changeSet);
	Q_UNUSED(notifier);

	if (state){
		m_mediaPlayer.play();
	}
	else{
		m_mediaPlayer.pause();
	}

	return true;
}


double CPhononVideoViewGuiComp::GetMediumLength() const
{
	EnsureSync();

	return m_mediaPlayer.duration() * 0.001;
}


double CPhononVideoViewGuiComp::GetCurrentPosition() const
{
	return m_currentPosition;
}


bool CPhononVideoViewGuiComp::SetCurrentPosition(double position)
{
	if (qAbs(position - GetCurrentPosition()) < I_BIG_EPSILON){
		return true;
	}

	if (m_mediaPlayer.isSeekable()){
		static const ChangeSet changeSet(CF_MEDIA_POSITION);
		istd::CChangeNotifier notifier(this, &changeSet);
		Q_UNUSED(notifier);

		m_mediaPlayer.setPosition(qint64(position * 1000));

		m_currentPosition = position;

		return true;
	}

	return false;
}


int CPhononVideoViewGuiComp::GetSupportedFeatures() const
{
	int flags = SF_OPEN_MEDIA | SF_PLAY | SF_AUTO_PLAY;

	if (m_mediaPlayer.isSeekable()){
		flags = flags | SF_SEEK;
	}

	return flags;
}


// reimplemented (imm::IVideoInfo)

int CPhononVideoViewGuiComp::GetFramesCount() const
{
	return int(GetMediumLength() * *m_framesPerSecondAttrPtr);
}


double CPhononVideoViewGuiComp::GetFrameIntervall() const
{
	return 1.0 / *m_framesPerSecondAttrPtr;
}


istd::CIndex2d CPhononVideoViewGuiComp::GetFrameSize() const
{
	return istd::CIndex2d::GetInvalid();
}


double CPhononVideoViewGuiComp::GetPixelAspectRatio() const
{
	return -1;
}


// reimplemented (imm::IVideoController)

int CPhononVideoViewGuiComp::GetCurrentFrame() const
{
	return int(GetCurrentPosition() * *m_framesPerSecondAttrPtr + 0.5);
}


bool CPhononVideoViewGuiComp::SetCurrentFrame(int frameIndex)
{
	return SetCurrentPosition(frameIndex / *m_framesPerSecondAttrPtr);
}


// reimplemented (ifile::IFileTypeInfo)

bool CPhononVideoViewGuiComp::GetFileExtensions(QStringList& result, const istd::IChangeable* /*dataObjectPtr*/, int flags, bool doAppend) const
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


QString CPhononVideoViewGuiComp::GetTypeDescription(const QString* /*extensionPtr*/) const
{
	return "Video files";
}


// protected methods

void CPhononVideoViewGuiComp::EnsureSync() const
{
	for (;;){
		QMediaPlayer::MediaStatus mediaStatus = m_mediaPlayer.mediaStatus();
		if ((mediaStatus == QMediaPlayer::BufferingMedia) || (mediaStatus == QMediaPlayer::LoadingMedia)){
			QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
		}
		else{
			break;
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CPhononVideoViewGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	m_mediaPlayer.setVideoOutput(GetQtWidget());
}


void CPhononVideoViewGuiComp::OnGuiDestroyed()
{
	CloseMedium();

	BaseClass::OnGuiDestroyed();
}


} // namespace iphonon


