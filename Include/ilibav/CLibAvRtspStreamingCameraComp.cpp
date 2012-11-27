#include "ilibav/CLibAvRtspStreamingCameraComp.h"


// Qt includes
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "imath/CGeneralUnitInfo.h"
#include "i2d/CRectangle.h"

// IACF includes
#include "ilibav/CLibAvConverter.h"


namespace ilibav
{


// public methods

CLibAvRtspStreamingCameraComp::CLibAvRtspStreamingCameraComp()
{	
	m_mutexPtr = NULL;	
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
	iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);

	if (bitmapPtr == NULL){
		return TS_OK;
	}

	if (!m_networkAccessManagerPtr.IsValid()){
		m_networkAccessManagerPtr.SetPtr(new QNetworkAccessManager());

		connect(	m_networkAccessManagerPtr.GetPtr(),
					SIGNAL(finished(QNetworkReply*)),
					this,
					SLOT(requestReceived(QNetworkReply*))
					);
	}

	if (!m_streamingClientPtr.IsValid()){
		m_streamingClientPtr.SetPtr(new CLibAvRtspStreamingClient());
	}	
	
	EnsureConnected(paramsPtr);	

	if(m_streamingClientPtr->RetrieveFrame(bitmapPtr))
		return TS_OK;
	
	return TS_INVALID;	
}

// reimplemented (imeas::INumericConstraints)

int CLibAvRtspStreamingCameraComp::GetNumericValuesCount() const
{
	return 1;
}


QString CLibAvRtspStreamingCameraComp::GetNumericValueName(int index) const
{
	switch (index){
	case 0:
		return QObject::tr("Framerate", "Framerate");

	default:
		return "";
	}
}


QString CLibAvRtspStreamingCameraComp::GetNumericValueDescription(int index) const
{
	switch (index){
	case 0:
		return QObject::tr("Framerate");
	
	default:
		return "";
	}
}


const imath::IUnitInfo& CLibAvRtspStreamingCameraComp::GetNumericValueUnitInfo(int index) const
{
	static imath::CGeneralUnitInfo frameRateUnitInfo(imath::IUnitInfo::UT_TECHNICAL, "fps", 100, istd::CRange(1, 100));

	switch (index){
	case 0:
		return frameRateUnitInfo;
	
	default:
		return frameRateUnitInfo;
	}	
}


// protected methods

void CLibAvRtspStreamingCameraComp::EnsureConnected(const iprm::IParamsSet* paramsPtr)
{
	QUrl cameraUrl;

	//read stream path
	if (m_urlParamsIdAttrPtr.IsValid()){
		const ifile::IFileNameParam* urlParamPtr = dynamic_cast<const ifile::IFileNameParam*>(paramsPtr->GetParameter(*m_urlParamsIdAttrPtr));
		if (urlParamPtr != NULL){
			cameraUrl.setUrl(urlParamPtr->GetPath());
		}
	}	

	if (cameraUrl.isValid() && m_defaultUrlParamCompPtr.IsValid()){
		cameraUrl = m_defaultUrlParamCompPtr->GetPath();
	}	

	//read AOI	
	if (m_aoiParamsIdAttrPtr.IsValid()){
		const i2d::CRectangle* aoiParamPtr = dynamic_cast<const i2d::CRectangle*>(paramsPtr->GetParameter(*m_aoiParamsIdAttrPtr));
		if (aoiParamPtr != NULL && aoiParamPtr->IsValidNonEmpty()){
			cameraUrl.addQueryItem("x0", QString::number(int(aoiParamPtr->GetLeft())));
			cameraUrl.addQueryItem("y0", QString::number(int(aoiParamPtr->GetTop())));
			cameraUrl.addQueryItem("x1", QString::number(int(aoiParamPtr->GetRight())));
			cameraUrl.addQueryItem("y1", QString::number(int(aoiParamPtr->GetBottom())));			
		}
	}

	//read framerate
	if(m_framerateParamsIdAttrPtr.IsValid()){
		const imeas::INumericValue* framerateParamPtr = dynamic_cast<const imeas::INumericValue*>(paramsPtr->GetParameter(*m_framerateParamsIdAttrPtr));
		if (framerateParamPtr != NULL){
			imath::CVarVector values = framerateParamPtr->GetValues();

			if(values.GetElementsCount() >= 1){
				cameraUrl.addQueryItem("fps", QString::number(int(values[0])));
			}
		}
	}

	//add suffix
	if(m_urlSuffixAttrPtr.IsValid() && !(*m_urlSuffixAttrPtr).isEmpty()){
		//splits into query item values 'key=value'
		QList<QByteArray> suffixValues = (*m_urlSuffixAttrPtr).split('&');

		for(		QList<QByteArray>::const_iterator iter = suffixValues.constBegin();
					iter != suffixValues.constEnd();
					++iter){

			//splits into key/value pairs
			QList<QByteArray> tuple = (*iter).split('=');
			if(tuple.size() == 2){
				cameraUrl.addQueryItem(tuple[0], tuple[1]);
			}
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
	if (!cameraUrl.isValid() && isBrightnessSet){
		//create http path from rtsp path
		QUrl httpUrl(cameraUrl.host());
		httpUrl.setScheme("http");
		httpUrl.setPath("/set?brightness=" + QString::number(brightness));

		QNetworkRequest request(httpUrl);	
		
		m_networkAccessManagerPtr->get(request);
	}

	if (cameraUrl != m_currentCameraUrl){
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
		//Close connection if exists
		m_streamingClientPtr->CloseConnection();		

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


