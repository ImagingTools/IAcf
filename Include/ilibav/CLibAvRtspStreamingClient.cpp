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

// Live555 includes
#include "BasicUsageEnvironment.hh"

#include "CLibAvRtspStreamingClient.h"
#include "CLibAvRtspStreamingDataSink.h"

namespace ilibav
{

// public methods

	CLibAvRtspStreamingClient::CLibAvRtspStreamingClient()
{
	scheduler = BasicTaskScheduler::createNew();
	env = BasicUsageEnvironment::createNew(*scheduler);
}

bool CLibAvRtspStreamingClient::OpenConnection(const QString& url)
{	
	QByteArray bytes_array = url.toAscii();
	char *c_url = bytes_array.data();
	RTSPClient* rtspClient = CLibAvRtspStreamingClient::CLibAvRtspConnection::createNew(this, *env, c_url, RTSP_CLIENT_VERBOSITY_LEVEL, NULL, 0);	
	if (rtspClient == NULL) {
		//Failed to create a RTSP client for URL
		return false; 		
	}

	// Next, send a RTSP "DESCRIBE" command, to get a SDP description for the stream.
	// Note that this command - like all RTSP commands - is sent asynchronously; we do not block, waiting for a response.
	// Instead, the following function call returns immediately, and we handle the RTSP response later, from within the event loop:
	rtspClient->sendDescribeCommand(&continueAfterDESCRIBE);
	
	// Start a new streaming thread
	this->start();

	return true;
}

void CLibAvRtspStreamingClient::CloseConnection()
{
	//Set loop variable to non-zero - end loop
	eventLoopWatchVariable = 1;
}

void CLibAvRtspStreamingClient::FrameArrived(AVFrame* frame, int width, int height, int pixelformat)
{
	Q_EMIT frameReady(frame, width, height, pixelformat);	
}

CLibAvRtspStreamingClient::~CLibAvRtspStreamingClient()
{
	env->reclaim(); 
	env = NULL;
    delete scheduler; 
	scheduler = NULL;
}

CLibAvRtspStreamingClient::CLibAvRtspConnection* CLibAvRtspStreamingClient::CLibAvRtspConnection::createNew(CLibAvRtspStreamingClient *streamClient, UsageEnvironment& env, char const* rtspURL,
																						  int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum)
{
	return new CLibAvRtspConnection(streamClient, env, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum);
}

CLibAvRtspStreamingClient::CLibAvRtspConnection::CLibAvRtspConnection(CLibAvRtspStreamingClient *streamClient, UsageEnvironment& env, char const* rtspURL,
															 int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum)
  : RTSPClient(env, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum)
{
	m_streamClient = streamClient;

	m_iter = NULL;
	m_session = NULL;
	m_subsession = NULL;
	m_streamTimerTask = NULL;
	m_duration = 0.0;	
}

CLibAvRtspStreamingClient::CLibAvRtspConnection::~CLibAvRtspConnection()
{
	delete m_iter;
	if (m_session != NULL) {
		// We also need to delete "session", and unschedule "streamTimerTask" (if set)
		UsageEnvironment& env = m_session->envir(); // alias
		env.taskScheduler().unscheduleDelayedTask(m_streamTimerTask);
		Medium::close(m_session);
	}
}

// protected methods

void CLibAvRtspStreamingClient::run()
{
	eventLoopWatchVariable = 0;

	// All subsequent activity takes place within the event loop:
	env->taskScheduler().doEventLoop(&eventLoopWatchVariable);
	// This function call does not return, unless, at some point in time, "eventLoopWatchVariable" gets set to something non-zero.
}

// response handlers

void CLibAvRtspStreamingClient::continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString) 
{
	do {
		UsageEnvironment& env = rtspClient->envir(); // alias
		CLibAvRtspConnection *conn = dynamic_cast<CLibAvRtspConnection*>(rtspClient);

		Q_ASSERT(conn != NULL);

		if (resultCode != 0) {
		  //Failed to get a SDP description
		  break;
		}

		char* const sdpDescription = resultString;		

		// Create a media session object from this SDP description:
		conn->m_session = MediaSession::createNew(env, sdpDescription);
		delete[] sdpDescription; // because we don't need it anymore
		if (conn->m_session == NULL) {
		  //Failed to create a MediaSession object from the SDP description
		  break;
		} else if (!conn->m_session->hasSubsessions()) {
		  //This session has no media subsessions 
		  break;
		}

		// Then, create and set up our data source objects for the session.  We do this by iterating over the session's 'subsessions',
		// calling "MediaSubsession::initiate()", and then sending a RTSP "SETUP" command, on each one.
		// (Each 'subsession' will have its own data source.)
		conn->m_iter = new MediaSubsessionIterator(*(conn->m_session));
		setupNextSubsession(rtspClient);
		return;
	} while (0);
	
	// An unrecoverable error occurred with this stream.
	shutdownStream(rtspClient);
}

void CLibAvRtspStreamingClient::setupNextSubsession(RTSPClient* rtspClient)
{
	CLibAvRtspConnection *conn = dynamic_cast<CLibAvRtspConnection*>(rtspClient);
	Q_ASSERT(conn != NULL);
	
	conn->m_subsession = conn->m_iter->next();
	
	if (conn->m_subsession != NULL) {
		if (!conn->m_subsession->initiate()) {
			//Failed to initiate the subsession
			setupNextSubsession(rtspClient); // give up on this subsession; go to the next one
		} else {
			//Initiated subsession			
			// Continue setting up this subsession, by sending a RTSP "SETUP" command:
			rtspClient->sendSetupCommand(*(conn->m_subsession), continueAfterSETUP);
		}
		return;
	}
	
	// We've finished setting up all of the subsessions.  Now, send a RTSP "PLAY" command to start the streaming:
	if (conn->m_session->absStartTime() != NULL) {
		// Special case: The stream is indexed by 'absolute' time, so send an appropriate "PLAY" command:
		rtspClient->sendPlayCommand(*(conn->m_session), continueAfterPLAY, conn->m_session->absStartTime(), conn->m_session->absEndTime());
	} else {
		conn->m_duration = conn->m_session->playEndTime() - conn->m_session->playStartTime();
		rtspClient->sendPlayCommand(*(conn->m_session), continueAfterPLAY);
	}
}

void CLibAvRtspStreamingClient::continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* /*resultString*/)
{
	do {
		UsageEnvironment& env = rtspClient->envir(); // alias
		CLibAvRtspConnection *conn = dynamic_cast<CLibAvRtspConnection*>(rtspClient);
		Q_ASSERT(conn != NULL);

		if (resultCode != 0) {
			//Failed to set up the subsession
			break;
		}
		
		// Having successfully setup the subsession, create a data sink for it, and call "startPlaying()" on it.
		// (This will prepare the data sink to receive data; the actual flow of data from the client won't start happening until later,
		// after we've sent a RTSP "PLAY" command.)
		
		conn->m_subsession->sink = CLibAvRtspStreamingDataSink::createNew(conn->m_streamClient, env, *(conn->m_subsession), rtspClient->url());		
		
		if (conn->m_subsession->sink == NULL) {
			//Failed to create a data sink for the subsession			
			break;
		}
		
		//Created a data sink for the subsession
		conn->m_subsession->miscPtr = rtspClient; // a hack to let subsession handle functions get the "RTSPClient" from the subsession 
		conn->m_subsession->sink->startPlaying(*(conn->m_subsession->readSource()),
			subsessionAfterPlaying, conn->m_subsession);
		
		// Also set a handler to be called if a RTCP "BYE" arrives for this subsession:
		if (conn->m_subsession->rtcpInstance() != NULL) {
			conn->m_subsession->rtcpInstance()->setByeHandler(subsessionByeHandler, conn->m_subsession);
		}
	} while (0);
	
	// Set up the next subsession, if any:
	setupNextSubsession(rtspClient);
}

void CLibAvRtspStreamingClient::continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* /*resultString*/)
{
	do {
		//CLibAvRtspConnection *conn = dynamic_cast<CLibAvRtspConnection*>(rtspClient);
		
		if (resultCode != 0) {
			//Failed to start playing session
			break;
		}
		
		// Set a timer to be handled at the end of the stream's expected duration (if the stream does not already signal its end
		// using a RTCP "BYE").  This is optional.  If, instead, you want to keep the stream active - e.g., so you can later
		// 'seek' back within it and do another RTSP "PLAY" - then you can omit this code.
		// (Alternatively, if you don't want to receive the entire stream, you could set this timer for some shorter value.)
		
		//if (conn->m_duration > 0) {
		//	unsigned const delaySlop = 2; // number of seconds extra to delay, after the stream's expected duration.  (This is optional.)
		//	conn->m_duration += delaySlop;
		//	unsigned uSecsToDelay = (unsigned)(conn->m_duration*1000000);
		//	conn->m_streamTimerTask = env.taskScheduler().scheduleDelayedTask(uSecsToDelay, (TaskFunc*)streamTimerHandler, rtspClient);
		//}
		
		//Started playing session
		/*if (conn->m_duration > 0) {
			env << " (for up to " << CLibAvRtspStreamingClient::duration << " seconds)";
		}*/
		
		return;
	} while (0);
	
	// An unrecoverable error occurred with this stream.
	shutdownStream(rtspClient);
}

void CLibAvRtspStreamingClient::subsessionAfterPlaying(void* clientData) 
{
	MediaSubsession* subsession = (MediaSubsession*)clientData;
	RTSPClient* rtspClient = (RTSPClient*)(subsession->miscPtr);
	
	// Begin by closing this subsession's stream:
	Medium::close(subsession->sink);
	subsession->sink = NULL;
	
	// Next, check whether *all* subsessions' streams have now been closed:
	MediaSession& session = subsession->parentSession();
	MediaSubsessionIterator iter(session);
	
	while ((subsession = iter.next()) != NULL) {
		if (subsession->sink != NULL) return; // this subsession is still active
	}
	
	// All subsessions' streams have now been closed, so shutdown the client:
	shutdownStream(rtspClient);
}

void CLibAvRtspStreamingClient::subsessionByeHandler(void* clientData) 
{
	MediaSubsession* subsession = (MediaSubsession*)clientData;
	//RTSPClient* rtspClient = (RTSPClient*)subsession->miscPtr;	
	
	// Received RTCP "BYE" on subsession
	// Now act as if the subsession had closed:
	
	subsessionAfterPlaying(subsession);
}

void CLibAvRtspStreamingClient::streamTimerHandler(void* clientData) 
{
	RTSPClient* rtspClient = (RTSPClient*)clientData;
	CLibAvRtspConnection *conn = dynamic_cast<CLibAvRtspConnection*>(rtspClient);
	conn->m_streamTimerTask = NULL;
	
	// Shut down the stream:
	shutdownStream(rtspClient);
}

void CLibAvRtspStreamingClient::shutdownStream(RTSPClient* rtspClient, int /*exitCode*/) 
{
	CLibAvRtspConnection *conn = dynamic_cast<CLibAvRtspConnection*>(rtspClient);
	
	// First, check whether any subsessions have still to be closed:
	if (conn->m_session != NULL) {
		Boolean someSubsessionsWereActive = False;
		MediaSubsessionIterator iter(*(conn->m_session));
		MediaSubsession* subsession;
		
		while ((subsession = iter.next()) != NULL) {
			if (subsession->sink != NULL) {
				Medium::close(subsession->sink);
				subsession->sink = NULL;
				
				if (subsession->rtcpInstance() != NULL) {
					subsession->rtcpInstance()->setByeHandler(NULL, NULL); // in case the server sends a RTCP "BYE" while handling "TEARDOWN"
				}
				
				someSubsessionsWereActive = True;
			}
		}
		
		if (someSubsessionsWereActive) {
			// Send a RTSP "TEARDOWN" command, to tell the server to shutdown the stream.
			// Don't bother handling the response to the "TEARDOWN".
			rtspClient->sendTeardownCommand(*(conn->m_session), NULL);
		}
	}

	//leave LIVE555 event loop - end of streaming	
	if(conn->m_streamClient != NULL){
		conn->m_streamClient->CloseConnection();
	}
	
	//Closing the stream
	Medium::close(rtspClient);
	// Note that this will also cause this stream's "StreamClientState" structure to get reclaimed.	
	
}

}
