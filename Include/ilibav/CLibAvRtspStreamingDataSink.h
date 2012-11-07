#ifndef ilibav_CLibAvRtspStreamingDataSink_included
#define ilibav_CLibAvRtspStreamingDataSink_included


#include <QMutex>

// Live555 includes
#include "MediaSink.hh"
#include "UsageEnvironment.hh"
#include "MediaSession.hh"

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

private:
	// called only by "createNew()"
	CLibAvRtspStreamingDataSink(
				CLibAvRtspStreamingClient* streamClient,
				UsageEnvironment& env,
				MediaSubsession& subsession);	

	virtual ~CLibAvRtspStreamingDataSink();
	
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
	//buffer for received frame
	QVector<u_int8_t> m_receiveBuffer;
	//stream and video (session) info
	MediaSubsession& m_subsession;

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

	//max buffer for incoming data
	static const int DATA_SINK_RECEIVE_BUFFER_SIZE = 500000;
};

}

#endif //ilibav_CLibAvRtspStreamingDataSink_included


