#include "ilibav/CLibAvRtspStreamingDataSink.h"


#include "ilibav/CLibAvConverter.h"


namespace ilibav
{


CLibAvRtspStreamingDataSink* CLibAvRtspStreamingDataSink::createNew(
			CLibAvRtspStreamingClient *streamClient,
			UsageEnvironment& env,
			MediaSubsession& subsession) 
{	
  return new CLibAvRtspStreamingDataSink(streamClient, env, subsession);
}


// private methods

CLibAvRtspStreamingDataSink::CLibAvRtspStreamingDataSink(
			CLibAvRtspStreamingClient *streamClient,
			UsageEnvironment& env,
			MediaSubsession& subsession)
:	MediaSink(env),
	m_receiveBuffer(DATA_SINK_RECEIVE_BUFFER_SIZE),
    m_subsession(subsession)
{
	m_streamClientPtr = streamClient;

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

	m_inputBufferPtr = (uint8_t*)av_malloc(DATA_SINK_RECEIVE_BUFFER_SIZE);

	m_spsUnitBufferSize = 0;
	m_ppsUnitBufferSize = 0;

	av_init_packet(&m_packet);

	//allocate frame
	m_framePtr = avcodec_alloc_frame();	
}


CLibAvRtspStreamingDataSink::~CLibAvRtspStreamingDataSink() 
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
}


void CLibAvRtspStreamingDataSink::afterGettingFrame(
			void* clientData,
			unsigned frameSize,
			unsigned numTruncatedBytes,
			struct timeval presentationTime,
			unsigned durationInMicroseconds) 
{
	CLibAvRtspStreamingDataSink* sink = (CLibAvRtspStreamingDataSink*)clientData;
	sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
}


void CLibAvRtspStreamingDataSink::afterGettingFrame(
			unsigned frameSize,
			unsigned /*numTruncatedBytes*/,
			struct timeval /*presentationTime*/,
			unsigned /*durationInMicroseconds*/)
{
	//check frame type
	int nalUnitType = m_receiveBuffer[0] & 0x1f; 

	if (nalUnitType == 7){
		//SPS NAL Unit
		std::memcpy(m_spsUnitBuffer, m_receiveBuffer.constData(), frameSize);
		m_spsUnitBufferSize = frameSize;
	}
	else if (nalUnitType == 8){
		//PPS NAL Unit
		std::memcpy(m_ppsUnitBuffer, m_receiveBuffer.constData(), frameSize);
		m_ppsUnitBufferSize = frameSize;
	}
	else{
		//video frame
		decodeFrame(frameSize);
	}	
	
	// Then continue, to request the next frame of data:
	continuePlaying();
}


void CLibAvRtspStreamingDataSink::decodeFrame(unsigned frameSize)
{
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
	
	std::memcpy(m_inputBufferPtr + usedBufferSize, m_receiveBuffer.constData(), frameSize);
	usedBufferSize += frameSize;

	m_packet.size = usedBufferSize;
	m_packet.data = m_inputBufferPtr;

	while (m_packet.size > 0){
		int gotFrame = 0;

		m_mutex.lock();
		int decodedLength = avcodec_decode_video2(m_codecContextPtr, m_framePtr, &gotFrame, &m_packet);
		m_mutex.unlock();
		
		if (decodedLength < 0){
			return;
		}

		if (gotFrame){
			m_streamClientPtr->FrameArrived(
						m_framePtr,
						m_codecContextPtr->width, m_codecContextPtr->height,
						(int)m_codecContextPtr->pix_fmt);
		}

		m_packet.size -= decodedLength;
		m_packet.data += decodedLength;
	}	
}


bool CLibAvRtspStreamingDataSink::continuePlaying() 
{
	
	if (fSource == NULL){
		return false; // sanity check (should not happen)
	}
	
	// Request the next frame of data from our input source.  "afterGettingFrame()" will get called later, when it arrives:
	
	fSource->getNextFrame(
				m_receiveBuffer.data(), m_receiveBuffer.size(),
				afterGettingFrame, this, onSourceClosure, this);

	return true;
}


}


