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

// Qt includes

#include <QUrl>
#include <QtNetwork/QNetworkReply>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "ilibav/CLibAvConverter.h"

#include "CLibAvRtspStreamingCameraComp.h"

namespace ilibav
{

// public methods

CLibAvRtspStreamingCameraComp::CLibAvRtspStreamingCameraComp()
{	
}

CLibAvRtspStreamingCameraComp::~CLibAvRtspStreamingCameraComp()
{	
}

void CLibAvRtspStreamingCameraComp::frameArrived(AVFrame* frame, int width, int height, int pixelformat)
{
	mutex.lock();

	frameSize = istd::CIndex2d(width, height);

	istd::CChangeNotifier notifier(m_frameBitmapPtr);

	CLibAvConverter::ConvertBitmap(*frame, frameSize, (AVPixelFormat)pixelformat, *m_frameBitmapPtr);

	mutex.unlock();
}

void CLibAvRtspStreamingCameraComp::requestReceived(QNetworkReply* /*networkReply*/)
{	
}

// reimplemented (icam::IBitmapAcquisition)

istd::CIndex2d CLibAvRtspStreamingCameraComp::GetBitmapSize(const iprm::IParamsSet* /*paramsPtr*/) const
{	
	return frameSize;
}

// reimplemented (iproc::IProcessor)

int CLibAvRtspStreamingCameraComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* /*inputPtr*/,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	m_frameBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);

	if(m_frameBitmapPtr == NULL){
		return TS_OK;
	}

	if(!m_networkAccessManagerPtr.IsValid()){
		m_networkAccessManagerPtr.SetPtr(new QNetworkAccessManager());

		connect(m_networkAccessManagerPtr.GetPtr(), SIGNAL(finished(QNetworkReply*)),
			this, SLOT(requestReceived(QNetworkReply*)));
	}

	ReadParams(paramsPtr);	

	if(!m_streamingClientPtr.IsValid()){
		m_streamingClientPtr.SetPtr(new CLibAvRtspStreamingClient());		

		connect(m_streamingClientPtr.GetPtr(), SIGNAL(frameReady(AVFrame*, int, int, int)),
			this, SLOT(frameArrived(AVFrame*, int , int, int)), Qt::QueuedConnection);		

	}

	if(m_streamingClientPtr->isRunning()){		
		//stop streaming thread
		m_streamingClientPtr->CloseConnection();		
		
		//wait till thread ends
		while(m_streamingClientPtr->isRunning()) ;
	}

	m_streamingClientPtr->OpenConnection(m_rtspUrl);

	return TS_OK;	
}

// protected methods

void CLibAvRtspStreamingCameraComp::ReadParams(const iprm::IParamsSet* paramsPtr)
{	
	//read stream path
	if (m_urlParamsIdAttrPtr.IsValid()){
		const iprm::IFileNameParam* urlParamPtr = dynamic_cast<const iprm::IFileNameParam*>(paramsPtr->GetParameter(*m_urlParamsIdAttrPtr));
		if (urlParamPtr != NULL){
			m_rtspUrl = urlParamPtr->GetPath();
		}
	}
	
	if(m_rtspUrl.isEmpty() && m_defaultUrlParamCompPtr.IsValid()){
		m_rtspUrl = m_defaultUrlParamCompPtr->GetPath();
	}
	

	//create http path from rtsp path
	QString urlString = m_rtspUrl;
	if(!m_rtspUrl.isEmpty()){		

		int index_start = urlString.indexOf("rtsp://");

		int index_end = -1;
		if(index_start != -1){
			urlString = urlString.right(urlString.count() - 7);
			index_end = urlString.indexOf('/');
		}

		if(index_end != -1){
			urlString = urlString.left(index_end);

			int index_port = urlString.indexOf(':');
			if(index_port != -1)
				urlString = urlString.left(index_port);

			urlString = "http://" + urlString;
		}		
	}	

	//read adjust params
	int brightness = 0;
	bool isBrightnessSet = false;
	if (m_defaultAdjustParamsCompPtr.IsValid()){
		brightness = m_defaultAdjustParamsCompPtr->GetOffsetFactor();
		isBrightnessSet = true;
	}

	if (m_adjustParamsIdAttrPtr.IsValid()){
		const imeas::ILinearAdjustParams* adjustParamsPtr = dynamic_cast<const imeas::ILinearAdjustParams*>(paramsPtr->GetParameter(*m_adjustParamsIdAttrPtr));
		if (adjustParamsPtr != NULL){
			brightness = adjustParamsPtr->GetOffsetFactor();
			isBrightnessSet = true;
		}
	}

	//set brightness using http
	if(!urlString.isEmpty() && isBrightnessSet){
		urlString.append("/set?brightness=");
		urlString.append(QString::number(brightness));

		QUrl url(urlString);		
		QNetworkRequest request(url);	
		
		m_networkAccessManagerPtr->get(request);
	}	
}

// reimplemented (icomp::CComponentBase)

void CLibAvRtspStreamingCameraComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();
}


void CLibAvRtspStreamingCameraComp::OnComponentDestroyed()
{
	if(m_streamingClientPtr.IsValid()){
		if(m_streamingClientPtr->isRunning()){			
			//stop streaming thread
			m_streamingClientPtr->CloseConnection();

			//wait till thread ends
			while(m_streamingClientPtr->isRunning()) ;
		}

		disconnect(m_streamingClientPtr.GetPtr(), SIGNAL(frameReady(AVFrame*, int, int, int)),
			this, SLOT(frameArrived(AVFrame*, int , int, int)));

		disconnect(m_networkAccessManagerPtr.GetPtr(), SIGNAL(finished(QNetworkReply*)),
			this, SLOT(requestReceived(QNetworkReply*)));
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace ilibav


