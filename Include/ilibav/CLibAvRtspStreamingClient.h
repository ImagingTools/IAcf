#ifndef ilibav_CLibAvRtspStreamingClient_included
#define ilibav_CLibAvRtspStreamingClient_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/QUrl>
#include <QMutex>

// Live555 includes
#include "liveMedia.hh"

// LIBAV includes
extern "C" {
#define __STDC_CONSTANT_MACROS
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"	
#include "libswscale/swscale.h"
#undef PixelFormat
#undef BYTES_PER_SAMPLE
#undef BITS_PER_SAMPLE
#undef WAVE_FORMAT_PCM
}

#include "ilibav/CLibAvConverter.h"


namespace ilibav
{

/**
	Class handles streaming rtsp client connection
*/
class CLibAvRtspStreamingClient: public QThread
{
	Q_OBJECT
public:
	
	CLibAvRtspStreamingClient();
	~CLibAvRtspStreamingClient();	

	/**
		Creates rtsp streming connection
	*/
	bool OpenConnection(const QUrl& url);

	/**
		Stops streaming loop (and connection)
	*/
	void CloseConnection(bool waitForClosed);

	/**
		New frame has arrived
	*/
	void DecodeFrame(u_int8_t* frameData, unsigned frameSize);	

	QMutex& GetMutex();

protected:
	/**
		Class representation of rtsp connection
	*/
	class CLibAvRtspConnection: public RTSPClient
	{
	public:
		MediaSubsessionIterator* m_subsessionIterPtr; 
		MediaSession* m_sessionPtr;
		MediaSubsession* m_subsessionPtr;
		TaskToken m_streamTimerTaskPtr;
		double m_duration;

		CLibAvRtspStreamingClient* m_streamClientPtr;

		static CLibAvRtspConnection* createNew(
					CLibAvRtspStreamingClient* streamClientPtr,
					UsageEnvironment& env,
					char const* rtspURL,
					int verbosityLevel = 0,
					char const* applicationName = NULL,
					portNumBits tunnelOverHTTPPortNum = 0);

	protected:
		CLibAvRtspConnection(
					CLibAvRtspStreamingClient* streamClientPtr,
					UsageEnvironment& env,
					char const* rtspURL,
					int verbosityLevel,
					char const* applicationName,
					portNumBits tunnelOverHTTPPortNum);

		virtual ~CLibAvRtspConnection(); 
	};

Q_SIGNALS:
	/**
		New frame has arrived
	*/
	void frameReady(AVFrame*, int , int, int);

protected:
	void run();

	//response handlers
	static void continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString);
	static void setupNextSubsession(RTSPClient* rtspClient);
	static void continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* resultString);
	static void continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* resultString);
	static void subsessionAfterPlaying(void* clientData);
	static void subsessionByeHandler(void* clientData);
	static void streamTimerHandler(void* clientData);
	static void shutdownStream(RTSPClient* rtspClient, int exitCode = 1);	

private:
	TaskScheduler* m_schedulerPtr;
	UsageEnvironment* m_environmentPtr;	

	/** 
		Indicates state of streaming loop:
		0 - in a loop and receiving
		non-zero - ends loop (and connection)
	*/
	char m_eventLoopWatchVariable;	
	
	//for ffmpeg decoding
	AVFormatContext* m_formatCtxPtr;	
	AVCodecContext* m_codecContextPtr;
	AVCodec* m_codecPtr;
	AVFrame* m_framePtr;	
	AVPacket m_packet;
		
	//buffers for decoding
	uint8_t* m_inputBufferPtr;
	uint8_t m_spsUnitBuffer[20];	
	int m_spsUnitBufferSize;
	uint8_t m_ppsUnitBuffer[20];
	int m_ppsUnitBufferSize;

	CLibAvRtspStreamingClient* m_streamClientPtr;

	QMutex m_mutex;

	static const int RTSP_CLIENT_VERBOSITY_LEVEL = 1;
};


}


#endif //ilibav_CLibAvRtspStreamingClient_included
