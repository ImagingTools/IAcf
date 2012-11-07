// Qt includes

#include <QUrl>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

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


void CLibAvRtspStreamingCameraComp::frameArrived(AVFrame* frame, int width, int height, int pixelformat)
{
	QMutexLocker locker(&m_mutex);

	m_lastImageSize = istd::CIndex2d(width, height);

	istd::CChangeNotifier notifier(m_frameBitmapPtr);

	CLibAvConverter::ConvertBitmap(*frame, m_lastImageSize, (AVPixelFormat)pixelformat, *m_frameBitmapPtr);
}


void CLibAvRtspStreamingCameraComp::requestReceived(QNetworkReply* /*networkReply*/)
{	
}


// reimplemented (icam::IBitmapAcquisition)

istd::CIndex2d CLibAvRtspStreamingCameraComp::GetBitmapSize(const iprm::IParamsSet* /*paramsPtr*/) const
{	
	return m_lastImageSize;
}

// reimplemented (iproc::IProcessor)

int CLibAvRtspStreamingCameraComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* /*inputPtr*/,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	m_frameBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);

	if (m_frameBitmapPtr == NULL){
		return TS_OK;
	}

	if (!m_networkAccessManagerPtr.IsValid()){
		m_networkAccessManagerPtr.SetPtr(new QNetworkAccessManager());

		connect(	m_networkAccessManagerPtr.GetPtr(),
					SIGNAL(finished(QNetworkReply*)),
					this,
					SLOT(requestReceived(QNetworkReply*)));
	}

	ReadParams(paramsPtr);	

	if (!m_streamingClientPtr.IsValid()){
		m_streamingClientPtr.SetPtr(new CLibAvRtspStreamingClient());		

		connect(	m_streamingClientPtr.GetPtr(),
					SIGNAL(frameReady(AVFrame*, int, int, int)),
					this,
					SLOT(frameArrived(AVFrame*, int , int, int)),
					Qt::QueuedConnection);

	}

	return TS_OK;	
}


// protected methods

void CLibAvRtspStreamingCameraComp::ReadParams(const iprm::IParamsSet* paramsPtr)
{
	QUrl cameraUrl;

	//read stream path
	if (m_urlParamsIdAttrPtr.IsValid()){
		const iprm::IFileNameParam* urlParamPtr = dynamic_cast<const iprm::IFileNameParam*>(paramsPtr->GetParameter(*m_urlParamsIdAttrPtr));
		if (urlParamPtr != NULL){
			cameraUrl.setUrl(urlParamPtr->GetPath());
		}
	}

	if (cameraUrl.isValid() && m_defaultUrlParamCompPtr.IsValid()){
		cameraUrl = m_defaultUrlParamCompPtr->GetPath();
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
	if (!cameraUrl.isValid() && isBrightnessSet){
		//create http path from rtsp path
		QUrl httpUrl(cameraUrl.host());
		httpUrl.setScheme("http");
		httpUrl.setPath("/set?brightness=" + QString::number(brightness));

		QNetworkRequest request(httpUrl);	
		
		m_networkAccessManagerPtr->get(request);
	}

	if (cameraUrl != m_currentCameraUrl){
		if (m_streamingClientPtr->isRunning()){		
			//stop streaming thread
			m_streamingClientPtr->CloseConnection(true);		
		}

		if (m_streamingClientPtr->OpenConnection(cameraUrl)){
			m_currentCameraUrl = cameraUrl;
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CLibAvRtspStreamingCameraComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();
}


void CLibAvRtspStreamingCameraComp::OnComponentDestroyed()
{
	if (m_streamingClientPtr.IsValid()){
		if (m_streamingClientPtr->isRunning()){			
			//stop streaming thread
			m_streamingClientPtr->CloseConnection(true);
		}

		disconnect(	m_streamingClientPtr.GetPtr(),
					SIGNAL(frameReady(AVFrame*, int, int, int)),
					this,
					SLOT(frameArrived(AVFrame*, int , int, int)));

		disconnect(	m_networkAccessManagerPtr.GetPtr(),
					SIGNAL(finished(QNetworkReply*)),
					this,
					SLOT(requestReceived(QNetworkReply*)));
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace ilibav


