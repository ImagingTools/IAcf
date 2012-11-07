#ifndef ilibav_CLibAvRtspStreamingDataSink_included
#define ilibav_CLibAvRtspStreamingDataSink_included

// Live555 includes
#include "MediaSink.hh"
#include "UsageEnvironment.hh"
#include "MediaSession.hh"

#include "ilibav/CLibAvRtspStreamingClient.h"

namespace ilibav
{

/**
	Class for receiving data from rtsp stream
*/
class CLibAvRtspStreamingDataSink: public MediaSink 
{
public:	
	static CLibAvRtspStreamingDataSink* createNew(
				CLibAvRtspStreamingClient *streamClient,
				UsageEnvironment& env,
				MediaSubsession& subsession); // identifies the kind of data that's being received

	//max buffer for incoming data
	static const int DATA_SINK_RECEIVE_BUFFER_SIZE = 500000;

private:
	// called only by "createNew()"
	CLibAvRtspStreamingDataSink(
				CLibAvRtspStreamingClient* streamClient,
				UsageEnvironment& env,
				MediaSubsession& subsession);	

	//virtual ~CLibAvRtspStreamingDataSink();
	
	static void afterGettingFrame(void* clientData, unsigned frameSize, 
		unsigned numTruncatedBytes, struct timeval presentationTime, 
		unsigned durationInMicroseconds);
	
	void afterGettingFrame(
				unsigned frameSize,
				unsigned numTruncatedBytes,
				struct timeval presentationTime,
				unsigned durationInMicroseconds);
	
	void decodeFrame(unsigned frameSize);

private:
	// redefined virtual functions:
	virtual bool continuePlaying();

private:	
	//stream and video (session) info
	MediaSubsession& m_subsession;	

	//buffer for received frame
	QVector<u_int8_t> m_receiveBuffer;

	CLibAvRtspStreamingClient* m_streamClientPtr;	
};

}

#endif //ilibav_CLibAvRtspStreamingDataSink_included


