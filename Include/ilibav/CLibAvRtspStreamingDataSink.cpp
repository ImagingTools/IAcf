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
	m_streamClientPtr->DecodeFrame(
		m_receiveBuffer.data(),
		frameSize);
	
	// Then continue, to request the next frame of data:
	continuePlaying();
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


