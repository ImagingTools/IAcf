#ifndef ilibav_CLibAvRtspStreamingCameraComp_included
#define ilibav_CLibAvRtspStreamingCameraComp_included


// LIBAV includes
extern "C"{
#define inline _inline
#define __STDC_CONSTANT_MACROS
#include <libavformat/avformat.h>
#undef inline
#undef __STDC_CONSTANT_MACROS
#undef PixelFormat
#undef BYTES_PER_SAMPLE
#undef BITS_PER_SAMPLE
#undef WAVE_FORMAT_PCM
}

// Qt includes
#include <QMutex>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

// ACF includes
#include "ibase/TLoggerCompWrap.h"
#include "iproc/TSyncProcessorWrap.h"
#include "iprm/IFileNameParam.h"

// ACF-Solutions includes
#include "icam/IBitmapAcquisition.h"
#include "icam/IExposureConstraints.h"
#include "icam/IExposureParams.h"
#include "imeas/ILinearAdjustParams.h"

#include "CLibAvRtspStreamingClient.h"


namespace ilibav
{

class CLibAvRtspStreamingCameraCompBase: 
	public ibase::CLoggerComponentBase,
	public QObject
{	
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CLibAvRtspStreamingCameraCompBase);
		I_ASSIGN(m_defaultUrlParamCompPtr, "DefaultUrlParam", "Default m_camera URL used if no URL is defined", false, "DefaultUrlParam");		
		I_ASSIGN(m_defaultAdjustParamsCompPtr, "DefaultAdjustParams", "Default m_contrast and m_brightness adjust parameters that will be used", false, "DefaultAdjustParams");		
	I_END_COMPONENT;

protected:
	I_REF(iprm::IFileNameParam, m_defaultUrlParamCompPtr);	
	I_REF(imeas::ILinearAdjustParams, m_defaultAdjustParamsCompPtr);
};


/**
	Implementation of icam::IBitmapAcquisition interface for streaming rtsp video using LibAv library for H.264 decoding.
*/
class CLibAvRtspStreamingCameraComp:	
	public CLibAvRtspStreamingCameraCompBase,
	virtual public iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>
{
	Q_OBJECT
		
public:
	typedef CLibAvRtspStreamingCameraCompBase BaseClass;	

	I_BEGIN_COMPONENT(CLibAvRtspStreamingCameraComp);
		I_REGISTER_INTERFACE(icam::IBitmapAcquisition);
		I_REGISTER_INTERFACE(icam::IExposureConstraints);		
		I_ASSIGN(m_urlParamsIdAttrPtr, "UrlParamId", "ID used to get m_camera URL from the parameter set", false, "UrlParamId");
		I_ASSIGN(m_adjustParamsIdAttrPtr, "AdjustParamsId", "ID used to get m_brightness and m_contrast adjust parameters from the parameter set", false, "AdjustParams");		
	I_END_COMPONENT;

	CLibAvRtspStreamingCameraComp();
	virtual ~CLibAvRtspStreamingCameraComp();	

	// reimplemented (icam::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);

public Q_SLOTS:
	void frameArrived(AVFrame*, int , int, int);
	void requestReceived(QNetworkReply*);

protected:
	/**
		Reads parameters from set 
	*/
	void ReadParams(const iprm::IParamsSet* paramsPtr);		

	// reimplemented (icomp::CComponentBase)
	void OnComponentCreated();
	void OnComponentDestroyed();

private:	
	I_ATTR(QByteArray, m_urlParamsIdAttrPtr);
	I_ATTR(QByteArray, m_adjustParamsIdAttrPtr);
	

	QString m_rtspUrl;	
	istd::TDelPtr<CLibAvRtspStreamingClient> m_streamingClientPtr;

	istd::CIndex2d frameSize;
	iimg::IBitmap* m_frameBitmapPtr;
	
	istd::TDelPtr<QNetworkAccessManager> m_networkAccessManagerPtr;

	QMutex mutex;
};

} // namespace ilibav

#endif // !ilibav_CLibAvRtspStreamingCameraComp_included


