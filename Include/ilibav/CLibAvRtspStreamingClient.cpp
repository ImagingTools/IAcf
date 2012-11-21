#include "ilibav/CLibAvRtspStreamingClient.h"

// Qt includes
#include <QtCore/QTime>

// Live555 includes
#include "BasicUsageEnvironment.hh"

// IACF includes
#include "ilibav/CLibAvRtspStreamingDataSink.h"


namespace ilibav
{


// public methods

CLibAvRtspStreamingClient::CLibAvRtspStreamingClient()	
{
	m_schedulerPtr = BasicTaskScheduler::createNew();
	m_environmentPtr = BasicUsageEnvironment::createNew(*m_schedulerPtr);
	currentRtspConnectionPtr = NULL;
	m_frameBitmapPtr = NULL;
	m_frameRetrieved = false;
	m_frameRetrieveTimeoutMs = 2000;

	//avlib codec initialization
	m_formatCtxPtr = NULL;
	m_codecContextPtr = NULL;
	m_codecPtr = NULL;
	m_framePtr = NULL;	

	av_register_all();	
	
	m_codecPtr = avcodec_find_decoder(AV_CODEC_ID_H264);
	if (!m_codecPtr){
		return;
	}

	m_codecContextPtr = avcodec_alloc_context3(m_codecPtr);
	if (!m_codecContextPtr){
		return;
	}	

	/* open it */
	if (avcodec_open2(m_codecContextPtr, m_codecPtr, NULL) < 0) {
		return;
	}	

	m_inputBufferPtr = (uint8_t*)av_malloc(CLibAvRtspStreamingDataSink::DATA_SINK_RECEIVE_BUFFER_SIZE);

	m_spsUnitBufferSize = 0;
	m_ppsUnitBufferSize = 0;

	av_init_packet(&m_packet);

	//allocate frame
	m_framePtr = avcodec_alloc_frame();	
}

bool CLibAvRtspStreamingClient::OpenConnection(const QUrl& url)
{
	//Close previous connection
	if(currentRtspConnectionPtr != NULL){
		shutdownStream(currentRtspConnectionPtr);
	}

	QByteArray urlByteArray = url.toEncoded();
	const char* urlText = urlByteArray.constData();	
	 
	currentRtspConnectionPtr = CLibAvRtspStreamingClient::CLibAvRtspConnection::createNew(this, *m_environmentPtr, urlText, RTSP_CLIENT_VERBOSITY_LEVEL, NULL, 0);	
	if (currentRtspConnectionPtr == NULL){
		//Failed to create a RTSP client for URL
		return false; 		
	}

	// Next, send a RTSP "DESCRIBE" command, to get a SDP description for the stream.
	// Note that this command - like all RTSP commands - is sent asynchronously; we do not block, waiting for a response.
	// Instead, the following function call returns immediately, and we handle the RTSP response later, from within the event loop:
	currentRtspConnectionPtr->sendDescribeCommand(&continueAfterDESCRIBE);

	// Start a new streaming thread
	start();

	return true;
}


void CLibAvRtspStreamingClient::CloseConnection()
{
	//Close previous connection
	if(currentRtspConnectionPtr != NULL){
		shutdownStream(currentRtspConnectionPtr);
		currentRtspConnectionPtr = NULL;
	}
}

void CLibAvRtspStreamingClient::QuitStreaming()
{
	//Set loop variable to non-zero - end loop
	m_eventLoopWatchVariable = 1;

	if(isRunning()){
		if (!wait(5000)){
			terminate();
		}		
	}
}


void CLibAvRtspStreamingClient::DecodeFrame(u_int8_t* frameData, unsigned frameSize)
{
	//check frame type
	int nalUnitType = frameData[0] & 0x1f; 

	if (nalUnitType == 7){
		//SPS NAL Unit
		std::memcpy(m_spsUnitBuffer, frameData, frameSize);
		m_spsUnitBufferSize = frameSize;
	}
	else if (nalUnitType == 8){
		//PPS NAL Unit
		std::memcpy(m_ppsUnitBuffer, frameData, frameSize);
		m_ppsUnitBufferSize = frameSize;
	}
	else{
		//video frame
		int usedBufferSize = 0;

		if (m_spsUnitBufferSize != 0){
			//Adding SPS Unit
			//first add 4 bytes for unit header 0x00000001
			m_inputBufferPtr[usedBufferSize++] = 0x00; 
			m_inputBufferPtr[usedBufferSize++] = 0x00;
			m_inputBufferPtr[usedBufferSize++] = 0x00;
			m_inputBufferPtr[usedBufferSize++] = 0x01;

			std::memcpy(m_inputBufferPtr + usedBufferSize, m_spsUnitBuffer, m_spsUnitBufferSize);		
			usedBufferSize += m_spsUnitBufferSize;

			m_spsUnitBufferSize = 0;
		}

		if (m_ppsUnitBufferSize != 0){
			//Adding PPS Unit
			//first add 4 bytes for unit header 0x00000001
			m_inputBufferPtr[usedBufferSize++] = 0x00;
			m_inputBufferPtr[usedBufferSize++] = 0x00;
			m_inputBufferPtr[usedBufferSize++] = 0x00;
			m_inputBufferPtr[usedBufferSize++] = 0x01;

			std::memcpy(m_inputBufferPtr + usedBufferSize, m_ppsUnitBuffer, m_ppsUnitBufferSize);		
			usedBufferSize += m_ppsUnitBufferSize;

			m_ppsUnitBufferSize = 0;
		}

		//Add video frame data
		//first add 4 bytes for unit header 0x00000001
		m_inputBufferPtr[usedBufferSize++] = 0x00;
		m_inputBufferPtr[usedBufferSize++] = 0x00;
		m_inputBufferPtr[usedBufferSize++] = 0x00;
		m_inputBufferPtr[usedBufferSize++] = 0x01;
		
		std::memcpy(m_inputBufferPtr + usedBufferSize, frameData, frameSize);
		usedBufferSize += frameSize;

		m_packet.size = usedBufferSize;
		m_packet.data = m_inputBufferPtr;

		while (m_packet.size > 0){
			int gotFrame = 0;

			int decodedLength = avcodec_decode_video2(m_codecContextPtr, m_framePtr, &gotFrame, &m_packet);			
			
			if (decodedLength < 0){				
				return;
			}

			if (gotFrame){
				QMutexLocker locker(&m_mutex);

				if(m_frameBitmapPtr != NULL){
					CLibAvConverter::ConvertBitmap(
						*m_framePtr,
						istd::CIndex2d(m_codecContextPtr->width, m_codecContextPtr->height),
						m_codecContextPtr->pix_fmt,
						*m_frameBitmapPtr);

					m_frameRetrieved = true;
				}				
			}
			

			m_packet.size -= decodedLength;
			m_packet.data += decodedLength;
		}
	}		
}

bool CLibAvRtspStreamingClient::RetrieveFrame(iimg::IBitmap* frameBitmap)
{	
	m_mutex.lock();

	m_frameRetrieved = false;
	m_frameBitmapPtr = frameBitmap;
	
	m_mutex.unlock();

	QTime timeoutTime = QTime::currentTime().addMSecs(m_frameRetrieveTimeoutMs);

	while(1){
		{
			QMutexLocker locker(&m_mutex);		

			if(m_frameRetrieved){
				m_frameBitmapPtr = NULL;
				return true;
			}

			//check for timeout
			if(QTime::currentTime() > timeoutTime){
				m_frameBitmapPtr = NULL;
				return false;		
			}
		}
	}
}

CLibAvRtspStreamingClient::~CLibAvRtspStreamingClient()
{
	//free buffer
	av_free(m_inputBufferPtr);	
	
	//free the YUV frame
	if (m_framePtr){
		avcodec_free_frame(&m_framePtr);
	}
	
	//close the codec
	if (m_codecContextPtr){
		avcodec_close(m_codecContextPtr);
		av_free(m_codecContextPtr);
	}
	
	//close the video file
	if (m_formatCtxPtr){
		av_close_input_file(m_formatCtxPtr);
	}
	
	m_environmentPtr->reclaim(); 
	m_environmentPtr = NULL;
    delete m_schedulerPtr; 
	m_schedulerPtr = NULL;
}


CLibAvRtspStreamingClient::CLibAvRtspConnection* CLibAvRtspStreamingClient::CLibAvRtspConnection::createNew(
			CLibAvRtspStreamingClient* streamClientPtr,
			UsageEnvironment& env,
			char const* rtspURL,
			int verbosityLevel,
			char const* applicationName,
			portNumBits tunnelOverHTTPPortNum)
{
	return new CLibAvRtspConnection(streamClientPtr, env, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum);
}


CLibAvRtspStreamingClient::CLibAvRtspConnection::CLibAvRtspConnection(
			CLibAvRtspStreamingClient* streamClientPtr,
			UsageEnvironment& env,
			char const* rtspURL,
			int verbosityLevel,
			char const* applicationName,
			portNumBits tunnelOverHTTPPortNum)
:	RTSPClient(env, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum)
{
	m_streamClientPtr = streamClientPtr;

	m_subsessionIterPtr = NULL;
	m_sessionPtr = NULL;
	m_subsessionPtr = NULL;
	m_streamTimerTaskPtr = NULL;
	m_duration = 0.0;	
}


CLibAvRtspStreamingClient::CLibAvRtspConnection::~CLibAvRtspConnection()
{
	delete m_subsessionIterPtr;
	if (m_sessionPtr != NULL) {
		// We also need to delete "session", and unschedule "streamTimerTask" (if set)
		UsageEnvironment& env = m_sessionPtr->envir(); // alias
		env.taskScheduler().unscheduleDelayedTask(m_streamTimerTaskPtr);
		Medium::close(m_sessionPtr);
	}
}


// protected methods

void CLibAvRtspStreamingClient::run()
{
	m_eventLoopWatchVariable = 0;

	// All subsequent activity takes place within the event loop:
	m_environmentPtr->taskScheduler().doEventLoop(&m_eventLoopWatchVariable);
	// This function call does not return, unless, at some point in time, "m_eventLoopWatchVariable" gets set to something non-zero.
}


// response handlers

void CLibAvRtspStreamingClient::continueAfterDESCRIBE(RTSPClient* rtspClientPtr, int resultCode, char* resultString) 
{
	do{
		UsageEnvironment& env = rtspClientPtr->envir(); // alias
		CLibAvRtspConnection *conn = dynamic_cast<CLibAvRtspConnection*>(rtspClientPtr);

		Q_ASSERT(conn != NULL);

		if (resultCode != 0) {
		  //Failed to get a SDP description
		  break;
		}

		char* const sdpDescription = resultString;		

		// Create a media session object from this SDP description:
		conn->m_sessionPtr = MediaSession::createNew(env, sdpDescription);
		delete[] sdpDescription; // because we don't need it anymore
		if (conn->m_sessionPtr == NULL) {
		  //Failed to create a MediaSession object from the SDP description
		  break;
		} else if (!conn->m_sessionPtr->hasSubsessions()) {
		  //This session has no media subsessions 
		  break;
		}

		// Then, create and set up our data source objects for the session.  We do this by iterating over the session's 'subsessions',
		// calling "MediaSubsession::initiate()", and then sending a RTSP "SETUP" command, on each one.
		// (Each 'subsession' will have its own data source.)
		conn->m_subsessionIterPtr = new MediaSubsessionIterator(*(conn->m_sessionPtr));
		setupNextSubsession(rtspClientPtr);
		return;
	} while (0);
	
	// An unrecoverable error occurred with this stream.
	shutdownStream(rtspClientPtr);
}


void CLibAvRtspStreamingClient::setupNextSubsession(RTSPClient* rtspClientPtr)
{
	CLibAvRtspConnection *conn = dynamic_cast<CLibAvRtspConnection*>(rtspClientPtr);
	Q_ASSERT(conn != NULL);
	
	conn->m_subsessionPtr = conn->m_subsessionIterPtr->next();
	
	if (conn->m_subsessionPtr != NULL) {
		if (!conn->m_subsessionPtr->initiate()) {
			//Failed to initiate the subsession
			setupNextSubsession(rtspClientPtr); // give up on this subsession; go to the next one
		} else {
			//Initiated subsession			
			// Continue setting up this subsession, by sending a RTSP "SETUP" command:
			rtspClientPtr->sendSetupCommand(*(conn->m_subsessionPtr), continueAfterSETUP);
		}
		return;
	}
	
	// We've finished setting up all of the subsessions.  Now, send a RTSP "PLAY" command to start the streaming:
	if (conn->m_sessionPtr->absStartTime() != NULL) {
		// Special case: The stream is indexed by 'absolute' time, so send an appropriate "PLAY" command:
		rtspClientPtr->sendPlayCommand(*(conn->m_sessionPtr), continueAfterPLAY, conn->m_sessionPtr->absStartTime(), conn->m_sessionPtr->absEndTime());
	} else {
		conn->m_duration = conn->m_sessionPtr->playEndTime() - conn->m_sessionPtr->playStartTime();
		rtspClientPtr->sendPlayCommand(*(conn->m_sessionPtr), continueAfterPLAY);
	}
}


void CLibAvRtspStreamingClient::continueAfterSETUP(RTSPClient* rtspClientPtr, int resultCode, char* /*resultString*/)
{
	do{
		UsageEnvironment& env = rtspClientPtr->envir(); // alias
		CLibAvRtspConnection *conn = dynamic_cast<CLibAvRtspConnection*>(rtspClientPtr);
		Q_ASSERT(conn != NULL);

		if (resultCode != 0) {
			//Failed to set up the subsession
			break;
		}
		
		// Having successfully setup the subsession, create a data sink for it, and call "startPlaying()" on it.
		// (This will prepare the data sink to receive data; the actual flow of data from the client won't start happening until later,
		// after we've sent a RTSP "PLAY" command.)
		
		conn->m_subsessionPtr->sink = CLibAvRtspStreamingDataSink::createNew(conn->m_streamClientPtr, env, *(conn->m_subsessionPtr));		
		
		if (conn->m_subsessionPtr->sink == NULL){
			//Failed to create a data sink for the subsession			
			break;
		}
		
		//Created a data sink for the subsession
		conn->m_subsessionPtr->miscPtr = rtspClientPtr; // a hack to let subsession handle functions get the "RTSPClient" from the subsession 
		conn->m_subsessionPtr->sink->startPlaying(
					*(conn->m_subsessionPtr->readSource()),
					subsessionAfterPlaying,
					conn->m_subsessionPtr);
		
		// Also set a handler to be called if a RTCP "BYE" arrives for this subsession:
		if (conn->m_subsessionPtr->rtcpInstance() != NULL){
			conn->m_subsessionPtr->rtcpInstance()->setByeHandler(subsessionByeHandler, conn->m_subsessionPtr);
		}
	} while (0);
	
	// Set up the next subsession, if any:
	setupNextSubsession(rtspClientPtr);
}


void CLibAvRtspStreamingClient::continueAfterPLAY(RTSPClient* rtspClientPtr, int resultCode, char* /*resultString*/)
{
	do {
		//CLibAvRtspConnection *conn = dynamic_cast<CLibAvRtspConnection*>(rtspClientPtr);
		
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
		//	conn->m_streamTimerTaskPtr = env.taskScheduler().scheduleDelayedTask(uSecsToDelay, (TaskFunc*)streamTimerHandler, rtspClientPtr);
		//}
		
		//Started playing session
		/*if (conn->m_duration > 0) {
			env << " (for up to " << CLibAvRtspStreamingClient::duration << " seconds)";
		}*/
		
		return;
	} while (0);
	
	// An unrecoverable error occurred with this stream.
	shutdownStream(rtspClientPtr);
}


void CLibAvRtspStreamingClient::subsessionAfterPlaying(void* clientData) 
{
	MediaSubsession* subsession = (MediaSubsession*)clientData;
	RTSPClient* rtspClientPtr = (RTSPClient*)(subsession->miscPtr);
	
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
	shutdownStream(rtspClientPtr);
}


void CLibAvRtspStreamingClient::subsessionByeHandler(void* clientData) 
{
	MediaSubsession* subsession = (MediaSubsession*)clientData;
	//RTSPClient* rtspClientPtr = (RTSPClient*)subsession->miscPtr;	
	
	// Received RTCP "BYE" on subsession
	// Now act as if the subsession had closed:
	
	subsessionAfterPlaying(subsession);
}

void CLibAvRtspStreamingClient::streamTimerHandler(void* clientData) 
{
	RTSPClient* rtspClientPtr = (RTSPClient*)clientData;
	CLibAvRtspConnection *conn = dynamic_cast<CLibAvRtspConnection*>(rtspClientPtr);
	conn->m_streamTimerTaskPtr = NULL;
	
	// Shut down the stream:
	shutdownStream(rtspClientPtr);
}


void CLibAvRtspStreamingClient::shutdownStream(RTSPClient* rtspClientPtr, int /*exitCode*/) 
{
	CLibAvRtspConnection *conn = dynamic_cast<CLibAvRtspConnection*>(rtspClientPtr);
	CLibAvRtspStreamingClient *client = conn->m_streamClientPtr;
	
	// First, check whether any subsessions have still to be closed:
	if (conn->m_sessionPtr != NULL) {
		Boolean someSubsessionsWereActive = False;
		MediaSubsessionIterator iter(*(conn->m_sessionPtr));
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
			rtspClientPtr->sendTeardownCommand(*(conn->m_sessionPtr), NULL);
		}
	}	

	//Closing the stream
	Medium::close(rtspClientPtr);	
	// Note that this will also cause this stream's "StreamClientState" structure to get reclaimed.

	//leave LIVE555 event loop - end of streaming	
	if (client != NULL){
		client->QuitStreaming();
	}
}


}


