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
** 	See http://www.ilena.org, write info@imagingtools.de or contact
**	by Skype to ACF_infoline for further information about the IACF.
**
********************************************************************************/

#ifndef ilibav_CLibAvRtspStreamingClient_included
#define ilibav_CLibAvRtspStreamingClient_included

// Qt includes
#include <QString>
#include <QThread>

// Live555 includes
#include "liveMedia.hh"

#include "ilibav/CLibAvConverter.h"


namespace ilibav
{

/**
	Class handles streaming rtsp client connection
*/
class CLibAvRtspStreamingClient  : public QThread
{
	Q_OBJECT
public:
	
	CLibAvRtspStreamingClient();
	~CLibAvRtspStreamingClient();	

	/**
		Creates rtsp streming connection
	*/
	bool OpenConnection(const QString& url);

	/**
		Stops streaming loop (and connection)
	*/
	void CloseConnection();

	/**
		New frame has arrived
	*/
	void FrameArrived(AVFrame* frame, int width, int height, int pixelformat);	

	/**
		Class representation of rtsp connection
	*/
	class CLibAvRtspConnection : public RTSPClient
	{
	public:
		MediaSubsessionIterator* m_iter; 
		MediaSession* m_session;
		MediaSubsession* m_subsession;
		TaskToken m_streamTimerTask;
		double m_duration;

		CLibAvRtspStreamingClient *m_streamClient;

		static CLibAvRtspConnection* createNew(CLibAvRtspStreamingClient *streamClient, UsageEnvironment& env, char const* rtspURL,
			int verbosityLevel = 0, char const* applicationName = NULL, portNumBits tunnelOverHTTPPortNum = 0);

	protected:
		CLibAvRtspConnection(CLibAvRtspStreamingClient *streamClient, UsageEnvironment& env, char const* rtspURL,
			int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum);

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
	TaskScheduler* scheduler;
	UsageEnvironment* env;	

	/** 
		Indicates state of streaming loop:
		0 - in a loop and receiving
		non-zero - ends loop (and connection)
	*/
	char eventLoopWatchVariable;

	static const int RTSP_CLIENT_VERBOSITY_LEVEL = 1;
};

}

#endif //ilibav_CLibAvRtspStreamingClient_included