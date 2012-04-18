/********************************************************************************
**
**	Copyright (C) 2007-2011 Witold Gantzke & Kirill Lepskiy
**
**	This file is part of the IACF Toolkit.
**
**	This file may be used under the terms of the GNU Lesser
**	General Public License version 2.1 as published by the Free Software
**	Foundation and appearing in the file LicenseLGPL.txt included in the
**	packaging of this file.  Please review the following information to
**	ensure the GNU Lesser General Public License version 2.1 requirements
**	will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**	If you are unsure which license is appropriate for your use, please
**	contact us at info@imagingtools.de.
**
** 	See http://www.imagingtools.de, write info@imagingtools.de or contact
**	by Skype to ACF_infoline for further information about the IACF.
**
********************************************************************************/


#ifndef ilibav_CLibAvVideoDecoderComp_included
#define ilibav_CLibAvVideoDecoderComp_included


// LIBAV includes
extern "C"{
#define inline _inline
#define __STDC_CONSTANT_MACROS
#include "libavformat/avformat.h"
#undef inline
}

// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "ibase/TLoggerCompWrap.h"

// ACF-Solutions includes
#include "icam/IBitmapAcquisition.h"
#include "imm/IVideoController.h"
#include "imeas/IDataSequence.h"
#include "imeas/ISampleAcquisition.h"


namespace ilibav
{


/**
	Implementation of imm::IVideoController and icam::IBitmapAcquisition interfaces using LibAv library.
*/
class CLibAvVideoDecoderComp:
			public ibase::CLoggerComponentBase,
			virtual public icam::IBitmapAcquisition,
			virtual public imeas::ISampleAcquisition,
			virtual public imm::IVideoController
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_CANNOT_OPEN = 0x56a20,
		MI_FORMAT_PROBLEM
	};

	I_BEGIN_COMPONENT(CLibAvVideoDecoderComp);
		I_REGISTER_INTERFACE(iproc::IProcessor);
		I_REGISTER_INTERFACE(icam::IBitmapAcquisition);
		I_REGISTER_INTERFACE(imeas::ISampleAcquisition);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(imm::IMediaController);
		I_REGISTER_INTERFACE(imm::IVideoInfo);
		I_REGISTER_INTERFACE(imm::IVideoController);
		I_ASSIGN(m_bitmapObjectCompPtr, "BitmapObject", "Bitmap object where current bitmap is stored", false, "Bitmap");
		I_ASSIGN(m_audioSampleObjectCompPtr, "AudioSampleObject", "Sample sequence object where current audio sample is stored", false, "AudioSequence");
		I_ASSIGN(m_tracePositionAttrPtr, "TracePosition", "If enabled, all position changes will be updated in preview objects", true, true);
		I_ASSIGN(m_grabProgressiveAttrPtr, "GrabProgressive", "If enabled the next frame will be returned by image and sample grab, if false only current frame will be returned", true, false);
		I_ASSIGN(m_minimalImageDistanceAttrPtr, "MinimalImageDistance", "Minimal distance between two grabbed images if progressive grabbing enabled", false, 0.1);
	I_END_COMPONENT;

	CLibAvVideoDecoderComp();
	virtual ~CLibAvVideoDecoderComp();

	// reimplemented (icam::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented (imeas::ISampleAcquisition)
	virtual double GetSamplingRate(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented (iproc::IProcessor)
	virtual int GetProcessorState(const iprm::IParamsSet* paramsPtr) const;
	virtual bool AreParamsAccepted(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				const istd::IChangeable* outputPtr) const;
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);
	virtual int BeginTask(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);
	virtual int WaitTaskFinished(
					int taskId = -1,
					double timeoutTime = -1,
					bool killOnTimeout = true);
	virtual void CancelTask(int taskId = -1);
	virtual int GetReadyTask();
	virtual int GetTaskState(int taskId = -1) const;
	virtual void InitProcessor(const iprm::IParamsSet* paramsPtr);

	// reimplemented (imm::IMediaController)
	virtual QString GetOpenedMediumUrl() const;
	virtual bool OpenMediumUrl(const QString& url, bool autoPlay = true);
	virtual void CloseMedium();
	virtual bool IsPlaying() const;
	virtual bool SetPlaying(bool state = true);
	virtual double GetMediumLength() const;
	virtual double GetCurrentPosition() const;
	virtual bool SetCurrentPosition(double position);
	virtual int	GetSupportedFeatures() const;

	// reimplemented (imm::IVideoInfo)
	virtual int GetFramesCount() const;
	virtual double GetFrameIntervall() const;
	virtual istd::CIndex2d GetFrameSize() const;
	virtual double GetPixelAspectRatio() const;

	// reimplemented (imm::IVideoController)
	virtual int GetCurrentFrame() const;
	virtual bool SetCurrentFrame(int frameIndex);

	// reimplemented (iser::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

protected:
	enum FrameType
	{
		FT_ERROR = -1,
		FT_END,
		FT_AUDIO_SAMPLE,
		FT_IMAGE,
		FT_SKIPPED_AUDIO_SAMPLE,
		FT_SKIPPED_IMAGE
	};

	FrameType ReadNextFrame(
				iimg::IBitmap* bitmapPtr,
				imeas::IDataSequence* audioSequencePtr,
				int minimalImageFrame = -1,
				int minimalAudioFrame = -1);

	/**
		Read next packet from stream.
	*/
	bool ReadNextPacket();

	/**
		Fininsh single task and return its ID.
	*/
	int FinishNextTask();

	bool TryTracePosition(
				int minimalImageFrame = -1,
				int minimalAudioFrame = -1);

	// reimplemented (icomp::CComponentBase)
	void OnComponentCreated();
	void OnComponentDestroyed();

private:
	struct ImageTask
	{
		int state;
		iimg::IBitmap* outputPtr;
	};
	typedef QMap<int, ImageTask> ImageTaskMap;
	ImageTaskMap m_imageTasks;

	struct AudioTask
	{
		int state;
		imeas::IDataSequence* outputPtr;
	};
	typedef QMap<int, AudioTask> AudioTaskMap;
	AudioTaskMap m_audioTasks;

	int m_videoStreamId;
	int m_audioStreamId;
	AVFormatContext* m_formatContextPtr;
	AVCodecContext* m_videoCodecContextPtr;
	AVCodecContext* m_audioCodecContextPtr;
	AVCodec* m_videoCodecPtr;
	AVCodec* m_audioCodecPtr;

	AVFrame* m_framePtr;
	AVFrame* m_frameRgbPtr;

	uint8_t* m_audioInputBuffer;
	int16_t* m_audioOutputBuffer;
	bool m_ignoreFirstAudioFrame;

	QVector<quint8> m_imageBuffer;

	AVPacket m_packet;
	int m_bytesRemaining;
	uint8_t* m_rawDataPtr;

	QString m_currentUrl;

	I_REF(iimg::IBitmap, m_bitmapObjectCompPtr);
	I_REF(imeas::IDataSequence, m_audioSampleObjectCompPtr);
	I_ATTR(bool, m_tracePositionAttrPtr);
	I_ATTR(bool, m_grabProgressiveAttrPtr);
	I_ATTR(double, m_minimalImageDistanceAttrPtr);

	int m_currentFrame;
	int m_lastReadFrame;

	bool m_isCurrentImageValid;
	bool m_isCurrentSampleValid;
	int m_nextTaskId;
};


} // namespace ilibav


#endif // !ilibav_CLibAvVideoDecoderComp_included

