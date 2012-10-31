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


#include "CLibAvRtspStreamingDataSink.h"
#include "CLibAvConverter.h"

namespace ilibav
{
	
CLibAvRtspStreamingDataSink* CLibAvRtspStreamingDataSink::createNew(CLibAvRtspStreamingClient *streamClient,
																	UsageEnvironment& env, MediaSubsession& subsession,
																	char const* streamId) 
{	
  return new CLibAvRtspStreamingDataSink(streamClient, env, subsession, streamId);
}

// private methods

CLibAvRtspStreamingDataSink::CLibAvRtspStreamingDataSink(CLibAvRtspStreamingClient *streamClient, UsageEnvironment& env,
														 MediaSubsession& subsession, char const* streamId)
  : MediaSink(env),
    m_fSubsession(subsession) 
{
	m_streamClient = streamClient;

	m_fStreamId = strDup(streamId);
	m_fReceiveBuffer = new u_int8_t[DATA_SINK_RECEIVE_BUFFER_SIZE];
	
	//avlib codec initialization
	m_pFormatCtx = NULL;
	m_pCodecCtx = NULL;
	m_pCodec = NULL;
	m_pFrame = NULL;	

	av_register_all();	
	
	m_pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
	if(!m_pCodec){
		return;
	}

	m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
	if(!m_pCodecCtx){
		return;
	}	

	/* open it */
	if (avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0) {
		return;
     }	

	m_inputBuffer = (uint8_t*)av_malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE + FF_INPUT_BUFFER_PADDING_SIZE);

	av_init_packet(&m_packet);

	//allocate frame
	m_pFrame = avcodec_alloc_frame();	
}

CLibAvRtspStreamingDataSink::~CLibAvRtspStreamingDataSink() 
{
	delete[] m_fReceiveBuffer;
	delete[] m_fStreamId;
	
	//free buffer
	av_free(m_inputBuffer);	
	
	//free the YUV frame
	if(m_pFrame)
		avcodec_free_frame(&m_pFrame);	
	
	//close the codec
	if(m_pCodecCtx){
		avcodec_close(m_pCodecCtx);
		av_free(m_pCodecCtx);
	}
	
	//close the video file
	if(m_pFormatCtx)
		av_close_input_file(m_pFormatCtx);
}

void CLibAvRtspStreamingDataSink::afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes,
				  struct timeval presentationTime, unsigned durationInMicroseconds) 
{
	CLibAvRtspStreamingDataSink* sink = (CLibAvRtspStreamingDataSink*)clientData;
	sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
}

void CLibAvRtspStreamingDataSink::afterGettingFrame(unsigned frameSize, unsigned /*numTruncatedBytes*/,
				  struct timeval /*presentationTime*/, unsigned /*durationInMicroseconds*/) 
{	
	decodeFrame(frameSize);
	
	// Then continue, to request the next frame of data:
	continuePlaying();
}

void CLibAvRtspStreamingDataSink::decodeFrame(unsigned frameSize)
{
	int got_frame = 0, len = 0;

	//first add 4 bytes for unit header 0x00000001
	m_inputBuffer[0] = 0x00;
	m_inputBuffer[1] = 0x00;
	m_inputBuffer[2] = 0x00;
	m_inputBuffer[3] = 0x01;
	
	memcpy(m_inputBuffer + 4, m_fReceiveBuffer, frameSize);

	m_packet.size = frameSize + 4;
	m_packet.data = m_inputBuffer;
	
	while (m_packet.size > 0) {
		
		mutex.lock();
		len = avcodec_decode_video2(m_pCodecCtx, m_pFrame, &got_frame, &m_packet);
		mutex.unlock();
		
		if (len < 0) {
			return;
		}
		
		if (got_frame) {

			m_streamClient->FrameArrived(m_pFrame, m_pCodecCtx->width,
				m_pCodecCtx->height, (int)m_pCodecCtx->pix_fmt);
		}

		m_packet.size -= len;
		m_packet.data += len;
	}	
}

bool CLibAvRtspStreamingDataSink::continuePlaying() 
{
	
	if (fSource == NULL) return False; // sanity check (should not happen)
	
	// Request the next frame of data from our input source.  "afterGettingFrame()" will get called later, when it arrives:
	
	fSource->getNextFrame(m_fReceiveBuffer, DATA_SINK_RECEIVE_BUFFER_SIZE,
		afterGettingFrame, this, onSourceClosure, this);
	
	return True;
}

}