#include "ilibav/CLibAvVideoDecoderComp.h"


// Qt includes
#include <QtCore/QStringList>


// ACF includes
#include "istd/CChangeNotifier.h"
#include "istd/TSmartPtr.h"
#include "ifile/IFileNameParam.h"

#include "imeas/CSamplesInfo.h"

#include "ilibav/CLibAvConverter.h"


namespace ilibav
{


// public methods

CLibAvVideoDecoderComp::CLibAvVideoDecoderComp()
:	m_videoStreamId(-1),
	m_audioStreamId(-1),
	m_formatContextPtr(NULL),
	m_videoCodecContextPtr(NULL),
	m_audioCodecContextPtr(NULL),
	m_videoCodecPtr(NULL),
	m_audioCodecPtr(NULL),
	m_framePtr(NULL),
	m_frameRgbPtr(NULL),
	m_rawDataSize(0),
	m_rawDataPtr(NULL),
	m_currentFrame(0),
	m_lastReadFrame(-1),
	m_isCurrentImageValid(false),
	m_isCurrentSampleValid(false),
	m_nextTaskId(0)
{
	av_register_all();

	m_packet.data = NULL;
	
	m_audioOutputBuffer = (int16_t*)av_malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE);

	m_ignoreFirstAudioFrame = false;
}


CLibAvVideoDecoderComp::~CLibAvVideoDecoderComp()
{	
	av_free(m_audioOutputBuffer);
}


// reimplemented (icam::IBitmapAcquisition)

istd::CIndex2d CLibAvVideoDecoderComp::GetBitmapSize(const iprm::IParamsSet* /*paramsPtr*/) const
{
	return GetFrameSize();
}


// reimplemented (imeas::ISampleAcquisition)

double CLibAvVideoDecoderComp::GetSamplingRate(const iprm::IParamsSet* /*paramsPtr*/) const
{
	if (m_audioCodecContextPtr != NULL){
		return m_audioCodecContextPtr->sample_rate;
	}

	return 0;
}


// reimplemented (iproc::IProcessor)

int CLibAvVideoDecoderComp::GetProcessorState(const iprm::IParamsSet* /*paramsPtr*/) const
{
	if ((m_videoCodecContextPtr != NULL) || (m_audioCodecContextPtr != NULL)){
		return PS_READY;
	}

	return PS_UNKNOWN;
}


bool CLibAvVideoDecoderComp::AreParamsAccepted(
			const iprm::IParamsSet* /*paramsPtr*/,
			const istd::IPolymorphic* /*inputPtr*/,
			const istd::IChangeable* outputPtr) const
{
	if (outputPtr != NULL){
		const iimg::IBitmap* bitmapPtr = dynamic_cast<const iimg::IBitmap*>(outputPtr);
		if (bitmapPtr != NULL){
			return true;
		}

		const imeas::IDataSequence* audioSequencePtr = dynamic_cast<const imeas::IDataSequence*>(outputPtr);
		if (audioSequencePtr != NULL){
			return true;
		}

		return false;
	}

	return true;
}


int CLibAvVideoDecoderComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* progressManagerPtr)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	int taskId = BeginTask(paramsPtr, inputPtr, outputPtr, progressManagerPtr);
	if (taskId >= 0){
		return WaitTaskFinished(taskId);
	}

	return TS_INVALID;
}


int CLibAvVideoDecoderComp::BeginTask(
			const iprm::IParamsSet* /*paramsPtr*/,
			const istd::IPolymorphic* /*inputPtr*/,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	m_nextTaskId = (m_nextTaskId + 1) & 0x7fff;

	iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (		(m_videoCodecContextPtr != NULL) &&
				(bitmapPtr != NULL) &&
				m_bitmapObjectCompPtr.IsValid()){
		ImageTask& task = m_imageTasks[m_nextTaskId];
		task.state = TS_WAIT;
		task.outputPtr = bitmapPtr;

		return m_nextTaskId;
	}

	imeas::IDataSequence* audioSequencePtr = dynamic_cast<imeas::IDataSequence*>(outputPtr);
	if (		(m_audioCodecContextPtr != NULL) &&
				(audioSequencePtr != NULL) &&
				m_audioSampleObjectCompPtr.IsValid()){
		AudioTask& task = m_audioTasks[m_nextTaskId];
		task.state = TS_WAIT;
		task.outputPtr = audioSequencePtr;

		return m_nextTaskId;
	}

	return -1;
}


int CLibAvVideoDecoderComp::WaitTaskFinished(
				int taskId,
				double /*timeoutTime*/,
				bool /*killOnTimeout*/)
{
	if (taskId >= 0){
		Q_ASSERT(GetTaskState(taskId) != TS_NONE);	// task exists

		while (GetTaskState(taskId) == TS_WAIT){
			if (FinishNextTask() < 0){
				m_imageTasks.remove(taskId);

				return TS_INVALID;
			}
		}

		int taskState = TS_NONE;

		ImageTaskMap::iterator imageIter = m_imageTasks.find(taskId);
		if (imageIter != m_imageTasks.end()){
			taskState = imageIter.value().state;

			m_imageTasks.erase(imageIter);
		}
		else{
			AudioTaskMap::iterator audioIter = m_audioTasks.find(taskId);
			if (audioIter != m_audioTasks.end()){
				taskState = audioIter.value().state;

				m_audioTasks.erase(audioIter);
			}
		}

		return taskState;
	}
	else{
		while (FinishNextTask() >= 0);

		int globalState = TS_NONE;
		for (		ImageTaskMap::const_iterator iter = m_imageTasks.begin();
					iter != m_imageTasks.end();
					++iter){
			globalState = qMax(globalState, iter.key());
		}
		for (		AudioTaskMap::const_iterator iter = m_audioTasks.begin();
					iter != m_audioTasks.end();
					++iter){
			globalState = qMax(globalState, iter.key());
		}

		m_imageTasks.clear();
		m_audioTasks.clear();

		return globalState;
	}
}


void CLibAvVideoDecoderComp::CancelTask(int taskId)
{
	if (taskId >= 0){
		m_imageTasks.remove(taskId);
		m_audioTasks.remove(taskId);

		if (m_imageTasks.isEmpty() && m_audioTasks.isEmpty()){
			m_nextTaskId = 0;
		}
	}
	else{
		m_imageTasks.clear();
		m_audioTasks.clear();

		m_nextTaskId = 0;
	}
}


int CLibAvVideoDecoderComp::GetReadyTask()
{
	for (		ImageTaskMap::const_iterator iter = m_imageTasks.begin();
				iter != m_imageTasks.end();
				++iter){
		const ImageTask& task = iter.value();
		if (task.state != TS_WAIT){
			return iter.key();
		}
	}

	for (		AudioTaskMap::const_iterator iter = m_audioTasks.begin();
				iter != m_audioTasks.end();
				++iter){
		const AudioTask& task = iter.value();
		if (task.state != TS_WAIT){
			return iter.key();
		}
	}

	return FinishNextTask();
}


int CLibAvVideoDecoderComp::GetTaskState(int taskId) const
{
	ImageTaskMap::ConstIterator imageIter = m_imageTasks.constFind(taskId);
	if (imageIter != m_imageTasks.constEnd()){
		const ImageTask& task = imageIter.value();

		return task.state;
	}

	AudioTaskMap::ConstIterator audioIter = m_audioTasks.constFind(taskId);
	if (audioIter != m_audioTasks.constEnd()){
		const AudioTask& task = audioIter.value();

		return task.state;
	}

	return TS_NONE;
}


void CLibAvVideoDecoderComp::InitProcessor(const iprm::IParamsSet* /*paramsPtr*/)
{
}


// reimplemented (imm::IMediaController)

QString CLibAvVideoDecoderComp::GetOpenedMediumUrl() const
{
	return m_currentUrl;
}


bool CLibAvVideoDecoderComp::OpenMediumUrl(const QString& url, bool /*autoPlay*/)
{
	ChangeSet changeSet(CF_STATUS, CF_MEDIA_POSITION);
	istd::CChangeNotifier notifier(this, changeSet);

	CloseMedium();

	if (		(avformat_open_input(&m_formatContextPtr, url.toLocal8Bit(), NULL, NULL) != 0) ||
				(m_formatContextPtr == NULL)){
		SendInfoMessage(MI_CANNOT_OPEN, QString("Cannot open media file ") + url);

		return false;
	}

	if (av_find_stream_info(m_formatContextPtr) < 0){
		SendInfoMessage(MI_FORMAT_PROBLEM, QString("No stream info for file ") + url);

		return false;
	}

	m_currentUrl = url;

	// Find the first video stream
	for (unsigned int i = 0; i < m_formatContextPtr->nb_streams; i++){
		AVCodecContext* codecContextPtr = m_formatContextPtr->streams[i]->codec;
		if (codecContextPtr == NULL){
			continue;
		}

		if (m_bitmapObjectCompPtr.IsValid() && (m_videoCodecPtr == NULL) && (codecContextPtr->codec_type == AVMEDIA_TYPE_VIDEO)){
			m_videoStreamId = i;
			m_videoCodecContextPtr = codecContextPtr;

			// Determine required buffer size and allocate buffer
			int numBytes = avpicture_get_size(
						PIX_FMT_RGB32,
						m_videoCodecContextPtr->width,
						m_videoCodecContextPtr->height);
			m_imageBuffer.resize(numBytes);

			// Assign appropriate parts of buffer to image planes in pFrameRGB
			avpicture_fill(
						(AVPicture*)m_frameRgbPtr,
						&m_imageBuffer[0],
						PIX_FMT_RGB32,
						m_videoCodecContextPtr->width,
						m_videoCodecContextPtr->height);

			// Find the decoder for the video stream
			m_videoCodecPtr = avcodec_find_decoder(m_videoCodecContextPtr->codec_id);
			if (m_videoCodecPtr == NULL){
				continue;
			}

			// Inform the codec that we can handle truncated bitstreams -- i.e.,
			// bitstreams where frame boundaries can fall in the middle of packets
			if (m_videoCodecPtr->capabilities & CODEC_CAP_TRUNCATED){
				m_videoCodecContextPtr->flags |= CODEC_FLAG_TRUNCATED;
			}

			// Open codec
			if (avcodec_open(m_videoCodecContextPtr, m_videoCodecPtr) < 0){
				SendInfoMessage(MI_FORMAT_PROBLEM, QString("Cannot open codec for file ") + url);

				m_videoCodecPtr = NULL;
			}
		}
		else if (m_audioSampleObjectCompPtr.IsValid() && (m_audioCodecPtr == NULL) && (codecContextPtr->codec_type == AVMEDIA_TYPE_AUDIO)){
			m_audioStreamId = i;

			// Find the decoder for the video stream
			m_audioCodecPtr = avcodec_find_decoder(codecContextPtr->codec_id);
			if (m_audioCodecPtr == NULL){
				continue;
			}

			// Inform the codec that we can handle truncated bitstreams -- i.e.,
			// bitstreams where frame boundaries can fall in the middle of packets
			if (m_audioCodecPtr->capabilities & CODEC_CAP_TRUNCATED){
				codecContextPtr->flags |= CODEC_FLAG_TRUNCATED;
			}

			// Open codec
			if (avcodec_open(codecContextPtr, m_audioCodecPtr) >= 0){
				m_audioCodecContextPtr = codecContextPtr;
				m_ignoreFirstAudioFrame = false;
			}
			else{
				SendInfoMessage(MI_FORMAT_PROBLEM, QString("Cannot open codec for file ") + url);

				m_audioCodecPtr = NULL;
			}
		}
	}

	if (m_bitmapObjectCompPtr.IsValid() && (m_videoCodecPtr == NULL)){
		SendInfoMessage(MI_FORMAT_PROBLEM, QString("No video decoder found for file ") + url);
	}

	if (m_audioSampleObjectCompPtr.IsValid() && (m_audioCodecPtr == NULL)){
		SendInfoMessage(MI_FORMAT_PROBLEM, QString("No audio decoder found for file ") + url);
	}

	m_ignoreFirstAudioFrame = true;

	TryTracePosition();

	return true;
}


void CLibAvVideoDecoderComp::CloseMedium()
{
	ChangeSet changeSet(CF_STATUS, CF_MEDIA_POSITION);
	istd::CChangeNotifier notifier(this, changeSet);

	if (m_formatContextPtr != NULL){
		av_close_input_file(m_formatContextPtr);

		m_formatContextPtr = NULL;
	}

	m_videoCodecContextPtr = NULL;
	m_audioCodecContextPtr = NULL;
	m_videoCodecPtr = NULL;
	m_audioCodecPtr = NULL;

	m_packet.data = NULL;
	m_rawDataSize = 0;
	m_rawDataPtr = NULL;

	m_currentFrame = 0;
	m_lastReadFrame = -1;

	m_isCurrentImageValid = false;
	m_isCurrentSampleValid = false;

	m_imageBuffer.clear();
}


bool CLibAvVideoDecoderComp::IsPlaying() const
{
	return false;
}


bool CLibAvVideoDecoderComp::SetPlaying(bool /*state*/)
{
	return false;
}


double CLibAvVideoDecoderComp::GetMediumLength() const
{
	if ((m_formatContextPtr != 0) && (m_videoStreamId >= 0)){
		AVStream* videoStreamPtr = m_formatContextPtr->streams[m_videoStreamId];
		if (videoStreamPtr != NULL){
			return double(videoStreamPtr->duration * videoStreamPtr->time_base.num) / videoStreamPtr->time_base.den;
		}
	}

	return 0;
}


double CLibAvVideoDecoderComp::GetCurrentPosition() const
{
	if ((m_formatContextPtr != 0) && (m_videoStreamId >= 0)){
		AVStream* videoStreamPtr = m_formatContextPtr->streams[m_videoStreamId];
		if (videoStreamPtr != NULL){
			return double(GetCurrentFrame() * videoStreamPtr->time_base.num) / videoStreamPtr->time_base.den;
		}
	}

	return 0;
}


bool CLibAvVideoDecoderComp::SetCurrentPosition(double position)
{
	if ((m_formatContextPtr != 0) && (m_videoStreamId >= 0)){
		AVStream* videoStreamPtr = m_formatContextPtr->streams[m_videoStreamId];
		if (videoStreamPtr != NULL){
			return SetCurrentFrame(int(position * videoStreamPtr->time_base.den / videoStreamPtr->time_base.num));
		}
	}

	return false;
}


int	CLibAvVideoDecoderComp::GetSupportedFeatures() const
{
	return SF_OPEN_MEDIA | SF_SEEK;
}


// reimplemented (imm::IVideoInfo)

int CLibAvVideoDecoderComp::GetFramesCount() const
{
	if ((m_formatContextPtr != 0) && (m_videoStreamId >= 0)){
		AVStream* videoStreamPtr = m_formatContextPtr->streams[m_videoStreamId];
		if (videoStreamPtr != NULL){
			return int(videoStreamPtr->duration);
		}
	}

	return 0;
}


double CLibAvVideoDecoderComp::GetFrameIntervall() const
{
	if ((m_formatContextPtr != 0) && (m_videoStreamId >= 0)){
		AVStream* videoStreamPtr = m_formatContextPtr->streams[m_videoStreamId];
		if ((videoStreamPtr != NULL) && (videoStreamPtr->time_base.den != 0)){
			return double(videoStreamPtr->time_base.num) / videoStreamPtr->time_base.den;
		}
	}

	return -1;
}


istd::CIndex2d CLibAvVideoDecoderComp::GetFrameSize() const
{
	if (m_videoCodecContextPtr != NULL){
		return istd::CIndex2d(m_videoCodecContextPtr->width, m_videoCodecContextPtr->height);
	}

	return istd::CIndex2d::GetInvalid();
}


double CLibAvVideoDecoderComp::GetPixelAspectRatio() const
{
	if ((m_videoCodecContextPtr != NULL) && (m_videoCodecContextPtr->sample_aspect_ratio.num != 0)){
		AVRational ratio = m_videoCodecContextPtr->sample_aspect_ratio;
		if (ratio.den != 0){
			return double(ratio.num) / ratio.den;
		}
	}

	return -1;
}


// reimplemented (imm::IVideoController)

int CLibAvVideoDecoderComp::GetCurrentFrame() const
{
	return m_currentFrame;
}


bool CLibAvVideoDecoderComp::SetCurrentFrame(int frameIndex)
{
	if (frameIndex == m_currentFrame){
		return true;
	}

	if ((m_formatContextPtr != 0) && (m_videoStreamId >= 0)){
		ChangeSet changeSet(CF_MEDIA_POSITION);
		istd::CChangeNotifier notifier(this, changeSet);

		if (av_seek_frame(
					m_formatContextPtr,
					m_videoStreamId,
					frameIndex,
					AVSEEK_FLAG_BACKWARD) >= 0){
			m_lastReadFrame = -1;
			m_ignoreFirstAudioFrame = true;

			if (!ReadNextPacket()){
				return false;
			}

			TryTracePosition(frameIndex);

			return true;
		}
	}

	return false;
}


// reimplemented (ifile::IFileTypeInfo)

bool CLibAvVideoDecoderComp::GetFileExtensions(QStringList& result, const istd::IChangeable* /*dataObjectPtr*/, int flags, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	if ((flags & QF_SAVE) != 0){
		result.push_back("mpg");
		result.push_back("avi");
	}

	return true;
}


QString CLibAvVideoDecoderComp::GetTypeDescription(const QString* /*extensionPtr*/) const
{
	return "Video files";
}


// protected methods

CLibAvVideoDecoderComp::FrameType CLibAvVideoDecoderComp::ReadNextFrame(
			iimg::IBitmap* bitmapPtr,
			imeas::IDataSequence* audioSequencePtr,
			int minimalImageFrame,
			int minimalAudioFrame)
{
	if (m_formatContextPtr == NULL){
		return FT_ERROR;
	}

	if (bitmapPtr != NULL){
		if (	(m_videoStreamId < 0) ||
				(m_videoCodecContextPtr == NULL) ||
				(m_framePtr == NULL) ||
				(m_frameRgbPtr == NULL)){
			return FT_ERROR;	// no video codec
		}
	}

	if (audioSequencePtr != NULL){
		if (	(m_audioStreamId < 0) ||
				(m_audioCodecContextPtr == NULL)){
			return FT_ERROR;	// no audio codec
		}
	}

	// Decode packets until we have decoded a complete frame
	while (true){
		if (m_packet.stream_index == m_videoStreamId){
			while ((m_rawDataPtr != NULL) && (m_packet.size > 0)){
				Q_ASSERT(m_videoCodecContextPtr != NULL);

				// Decode the next chunk of data
				int frameFinished = 0;				

				int bytesDecoded = avcodec_decode_video2(
							m_videoCodecContextPtr,
							m_framePtr,
							&frameFinished,
							&m_packet);

				// Was there an error?
				if (bytesDecoded > 0){

					m_packet.size -= bytesDecoded;
					m_packet.data += bytesDecoded;

					if (minimalImageFrame >= 0){
						if (m_packet.dts == AV_NOPTS_VALUE){
							return FT_SKIPPED_IMAGE;
						}

						if (m_currentFrame < minimalImageFrame){
							return FT_SKIPPED_IMAGE;
						}
					}

					// Did we finish the current frame? Then we can return
					if ((bitmapPtr != NULL) && (frameFinished != 0)){
						istd::CChangeNotifier notifier(bitmapPtr);

						if (CLibAvConverter::ConvertBitmap(
									*m_framePtr,
									GetFrameSize(),
									m_videoCodecContextPtr->pix_fmt,
									*bitmapPtr)){
							return FT_IMAGE;
						}
						else{
							return FT_ERROR;
						}
					}

					continue;
				}
				else{
					break;
				}
			}
		}
		else if (m_packet.stream_index == m_audioStreamId){
			while ((m_rawDataPtr != NULL) && (m_packet.size > 0)){
				Q_ASSERT(m_audioCodecContextPtr != NULL);
				
				// Decode the next chunk of data
				int audioBufferSize = AVCODEC_MAX_AUDIO_FRAME_SIZE;				

				int bytesDecoded = avcodec_decode_audio3(
							m_audioCodecContextPtr,
							m_audioOutputBuffer,
							&audioBufferSize,
							&m_packet);

				// Was there an error?
				if (bytesDecoded > 0){
					m_packet.size -= bytesDecoded;
					m_packet.data += bytesDecoded;

					if (minimalAudioFrame >= 0){
						if (m_packet.dts == AV_NOPTS_VALUE){
							return FT_SKIPPED_AUDIO_SAMPLE;
						}

						if (m_packet.dts < minimalAudioFrame){
							return FT_SKIPPED_AUDIO_SAMPLE;
						}
					}

					if ((audioSequencePtr != NULL) && !m_ignoreFirstAudioFrame){
						int sampleRawSize = 0;

						switch (m_audioCodecContextPtr->sample_fmt){
						case AV_SAMPLE_FMT_U8:
							sampleRawSize = sizeof(quint8);
							break;

						case AV_SAMPLE_FMT_S16:
							sampleRawSize = sizeof(qint16);
							break;

						case AV_SAMPLE_FMT_S32:
							sampleRawSize = sizeof(qint32);
							break;

						case AV_SAMPLE_FMT_FLT:
							sampleRawSize = sizeof(float);
							break;

						case AV_SAMPLE_FMT_DBL:
							sampleRawSize = sizeof(double);
							break;

						default:
							return FT_ERROR;	// sample format not supported
						}

						istd::CChangeNotifier notifier(audioSequencePtr);

						int channelsCount = m_audioCodecContextPtr->channels;
						int samplesCount = audioBufferSize / (sampleRawSize * channelsCount);

						istd::CRange resultProportionRange(0, double(samplesCount) / m_audioCodecContextPtr->sample_rate);
						istd::TSmartPtr<const imeas::IDataSequenceInfo> sequenceInfoPtr(new imeas::CSamplesInfo(resultProportionRange));
						if (!audioSequencePtr->CreateSequenceWithInfo(
									sequenceInfoPtr,
									samplesCount,
									channelsCount)){
							return FT_ERROR;
						}

						int allSamplesCount = samplesCount * channelsCount;

						switch (m_audioCodecContextPtr->sample_fmt){
						case AV_SAMPLE_FMT_U8:
							{
								quint8* samplesPtr = (quint8*)m_audioOutputBuffer;
								for (int i = 0; i < allSamplesCount; ++i){
									audioSequencePtr->SetSample(i / channelsCount, i % channelsCount, samplesPtr[i] / 127.5 - 1);
								}
							}
							break;

						case AV_SAMPLE_FMT_S16:
							{
								qint16* samplesPtr = (qint16*)m_audioOutputBuffer;
								for (int i = 0; i < allSamplesCount; ++i){
									audioSequencePtr->SetSample(i / channelsCount, i % channelsCount, samplesPtr[i] / 32768.0);
								}
							}
							break;

						case AV_SAMPLE_FMT_S32:
							{
								qint32* samplesPtr = (qint32*)m_audioOutputBuffer;
								for (int i = 0; i < allSamplesCount; ++i){
									audioSequencePtr->SetSample(i / channelsCount, i % channelsCount, double(samplesPtr[i]) / 0x80000000);
								}
							}
							break;

						case AV_SAMPLE_FMT_FLT:
							{
								float* samplesPtr = (float*)m_audioOutputBuffer;
								for (int i = 0; i < allSamplesCount; ++i){
									audioSequencePtr->SetSample(i / channelsCount, i % channelsCount, samplesPtr[i]);
								}
							}
							break;

						case AV_SAMPLE_FMT_DBL:
							{
								double* samplesPtr = (double*)m_audioOutputBuffer;
								for (int i = 0; i < allSamplesCount; ++i){
									audioSequencePtr->SetSample(i / channelsCount, i % channelsCount, samplesPtr[i]);
								}
							}
							break;
						}

						return FT_AUDIO_SAMPLE;
					}

					m_ignoreFirstAudioFrame = false;
				}
				else{
					break;
				}
			}
		}

		if (!ReadNextPacket()){
			return FT_ERROR;
		}
	}

	return FT_ERROR;
}



bool CLibAvVideoDecoderComp::ReadNextPacket()
{
	if (m_packet.data != NULL){
		m_packet.data = m_rawDataPtr;
		m_packet.size = m_rawDataSize;
		av_free_packet(&m_packet);
		m_packet.data = NULL;
		m_rawDataPtr = NULL;
	}

	// Read new packet
	if ((av_read_frame(m_formatContextPtr, &m_packet) >= 0) && (m_packet.data != NULL)){
		int processedFrame = int(m_packet.dts);
		if (processedFrame != m_currentFrame){
			ChangeSet changeSet(CF_MEDIA_POSITION);
			istd::CChangeNotifier posNotifier(this, changeSet);

			m_currentFrame = processedFrame;
		}
	}
	else{
		return false;
	}

	m_rawDataSize = m_packet.size;
	m_rawDataPtr = m_packet.data;

	return true;
}


int CLibAvVideoDecoderComp::FinishNextTask()
{
	int openTasksCount = 0;

	if (m_bitmapObjectCompPtr.IsValid() && (m_videoCodecContextPtr != NULL)){
		for (		ImageTaskMap::const_iterator iter = m_imageTasks.begin();
					iter != m_imageTasks.end();
					++iter){
			const ImageTask& task = iter.value();
			if (task.state == TS_WAIT){
				++openTasksCount;
			}
		}
	}

	if (m_audioSampleObjectCompPtr.IsValid() && (m_audioCodecContextPtr != NULL)){
		for (		AudioTaskMap::const_iterator iter = m_audioTasks.begin();
					iter != m_audioTasks.end();
					++iter){
			const AudioTask& task = iter.value();
			if (task.state == TS_WAIT){
				++openTasksCount;
			}
		}
	}

	while (openTasksCount > 0){
		if (m_isCurrentImageValid){
			Q_ASSERT(m_bitmapObjectCompPtr.IsValid());	// contains is valid, pointer must be also valid

			for (		ImageTaskMap::iterator iter = m_imageTasks.begin();
						iter != m_imageTasks.end();
						++iter){
				ImageTask& task = iter.value();
				Q_ASSERT(task.outputPtr != NULL);

				if (task.state == TS_WAIT){
					// Optimized for speed
					if (task.outputPtr == m_bitmapObjectCompPtr.GetPtr()){
						task.state = TS_OK;
					}
					else if (task.outputPtr->CopyFrom(*m_bitmapObjectCompPtr)){
						task.state = TS_OK;
					}
					else{
						task.state = TS_INVALID;
					}

					m_isCurrentImageValid = false;

					return iter.key();
				}
			}
		}

		if (m_isCurrentSampleValid){
			Q_ASSERT(m_audioSampleObjectCompPtr.IsValid());	// contains is valid, pointer must be also valid

			for (		AudioTaskMap::iterator iter = m_audioTasks.begin();
						iter != m_audioTasks.end();
						++iter){
				AudioTask& task = iter.value();
				Q_ASSERT(task.outputPtr != NULL);

				if (task.state == TS_WAIT){
					if (task.outputPtr == m_audioSampleObjectCompPtr.GetPtr()){
						task.state = TS_OK;
					}
					else if (task.outputPtr->CopyFrom(*m_audioSampleObjectCompPtr)){
						task.state = TS_OK;
					}
					else{
						task.state = TS_INVALID;
					}

					m_isCurrentSampleValid = false;

					return iter.key();
				}
			}
		}

		int nextImageFrame = 0;

		if (m_lastReadFrame >= 0){
			nextImageFrame = m_lastReadFrame + 1;

			if (m_minimalImageDistanceAttrPtr.IsValid() && (m_formatContextPtr != 0) && (m_videoStreamId >= 0)){
				AVStream* videoStreamPtr = m_formatContextPtr->streams[m_videoStreamId];
				if (videoStreamPtr != NULL){
					nextImageFrame = qMax(
								m_lastReadFrame + int(*m_minimalImageDistanceAttrPtr * videoStreamPtr->time_base.den / videoStreamPtr->time_base.num),
								nextImageFrame);
				}
			}
		}

		FrameType readFrameType = ReadNextFrame(
					m_bitmapObjectCompPtr.GetPtr(),
					m_audioSampleObjectCompPtr.GetPtr(),
					nextImageFrame);

		if (readFrameType == FT_IMAGE){
			Q_ASSERT(m_bitmapObjectCompPtr.IsValid());	// contains is valid, pointer must be also valid

			m_isCurrentImageValid = true;

			m_lastReadFrame = m_currentFrame;
		}
		else if (readFrameType == FT_AUDIO_SAMPLE){
			Q_ASSERT(m_audioSampleObjectCompPtr.IsValid());	// contains is valid, pointer must be also valid

			m_isCurrentSampleValid = true;
		}
		else if ((readFrameType != FT_SKIPPED_IMAGE) && (readFrameType != FT_SKIPPED_AUDIO_SAMPLE)){
			break;
		}
	}

	return -1;
}


bool CLibAvVideoDecoderComp::TryTracePosition(
				int minimalImageFrame,
				int minimalAudioFrame)
{
	if (*m_tracePositionAttrPtr){
		bool needReadImage = m_bitmapObjectCompPtr.IsValid();
		bool needReadSequence = m_audioSampleObjectCompPtr.IsValid();
		while (needReadImage || needReadSequence){
			FrameType readFrameType = ReadNextFrame(
						needReadImage? m_bitmapObjectCompPtr.GetPtr(): NULL,
						needReadSequence? m_audioSampleObjectCompPtr.GetPtr(): NULL,
						minimalImageFrame,
						minimalAudioFrame);

			switch (readFrameType){
			case FT_ERROR:
				return false;

			case FT_AUDIO_SAMPLE:
				needReadSequence = false;
				m_isCurrentSampleValid = true;
				break;

			case FT_IMAGE:
				needReadImage = false;
				m_isCurrentImageValid = true;
				break;
			}
		}
	}
	else{
		if (m_bitmapObjectCompPtr.IsValid()){
			m_bitmapObjectCompPtr->ResetImage();
		}

		if (m_audioSampleObjectCompPtr.IsValid()){
			m_audioSampleObjectCompPtr->ResetSequence();
		}
	}

	return true;
}


// reimplemented (icomp::CComponentBase)

void CLibAvVideoDecoderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapObjectCompPtr.IsValid()){
		m_framePtr = avcodec_alloc_frame();
		m_frameRgbPtr = avcodec_alloc_frame();
	}
}


void CLibAvVideoDecoderComp::OnComponentDestroyed()
{
	CloseMedium();

	if (m_framePtr != NULL){
		av_free(m_framePtr);

		m_framePtr = NULL;
	}

	if (m_frameRgbPtr != NULL){
		av_free(m_frameRgbPtr);

		m_frameRgbPtr = NULL;
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace ilibav


