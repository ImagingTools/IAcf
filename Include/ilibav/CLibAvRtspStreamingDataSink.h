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
}

#include "CLibAvRtspStreamingClient.h"

#ifndef ilibav_CLibAvRtspStreamingDataSink_included
#define ilibav_CLibAvRtspStreamingDataSink_included

namespace ilibav
{

/**
	Class for receiving data from rtsp stream
*/
class CLibAvRtspStreamingDataSink: public MediaSink 
{	

public:	
	static CLibAvRtspStreamingDataSink* createNew(CLibAvRtspStreamingClient *streamClient, UsageEnvironment& env,
		MediaSubsession& subsession, // identifies the kind of data that's being received
		char const* streamId = NULL); // identifies the stream itself (optional)		

private:
	// called only by "createNew()"
	CLibAvRtspStreamingDataSink(CLibAvRtspStreamingClient *streamClient, UsageEnvironment& env,
		MediaSubsession& subsession, char const* streamId);	

	virtual ~CLibAvRtspStreamingDataSink();
	
	static void afterGettingFrame(void* clientData, unsigned frameSize, 
		unsigned numTruncatedBytes, struct timeval presentationTime, 
		unsigned durationInMicroseconds);
	
	void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,struct timeval presentationTime,
		unsigned durationInMicroseconds);
	
	void decodeFrame(unsigned frameSize);

private:
	// redefined virtual functions:
	virtual bool continuePlaying();

private:

	//buffer for received frame
	u_int8_t* m_fReceiveBuffer;
	//stream and video (session) info
	MediaSubsession& m_fSubsession;
	//stream id
	char* m_fStreamId;

	//for ffmpeg decoding
	AVFormatContext *m_pFormatCtx;	
	AVCodecContext *m_pCodecCtx;
	AVCodec *m_pCodec;
	AVFrame *m_pFrame;	
	AVPacket m_packet;
		
	//buffers for decoding
	uint8_t *m_inputBuffer;
	uint8_t m_spsUnitBuffer[20];	
	int m_spsUnitBufferSize;
	uint8_t m_ppsUnitBuffer[20];
	int m_ppsUnitBufferSize;

	CLibAvRtspStreamingClient *m_streamClient;

	QMutex mutex;

  //max buffer for incoming data
  static const int DATA_SINK_RECEIVE_BUFFER_SIZE = 500000;
};

}

#endif //ilibav_CLibAvRtspStreamingDataSink_included