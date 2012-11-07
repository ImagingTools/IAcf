#ifndef ilibav_CLibAvRtspStreamingClient_included
#define ilibav_CLibAvRtspStreamingClient_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/QUrl>

// Live555 includes
#include "liveMedia.hh"

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
	void FrameArrived(AVFrame* frame, int width, int height, int pixelformat);	

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

	static const int RTSP_CLIENT_VERBOSITY_LEVEL = 1;
};


}


#endif //ilibav_CLibAvRtspStreamingClient_included
