#ifndef ilibav_CLibAvRtspStreamingCameraComp_included
#define ilibav_CLibAvRtspStreamingCameraComp_included


// Qt includes
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>

// ACF includes
#include "istd/CIndex2d.h"
#include "ibase/TLoggerCompWrap.h"
#include "iproc/TSyncProcessorWrap.h"
#include "ifile/IFileNameParam.h"

// ACF-Solutions includes
#include "icam/IBitmapAcquisition.h"
#include "icam/IExposureConstraints.h"
#include "icam/IExposureParams.h"
#include "imeas/ILinearAdjustParams.h"
#include "imeas/INumericValue.h"
#include "imeas/INumericConstraints.h"

#include "ilibav/CLibAvRtspStreamingClient.h"


namespace ilibav
{


/**
	Implementation of icam::IBitmapAcquisition interface for streaming rtsp video using LibAv library for H.264 decoding.
*/
class CLibAvRtspStreamingCameraComp:
			public QObject,
			public ibase::CLoggerComponentBase,
			virtual public iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>
{
	Q_OBJECT

public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CLibAvRtspStreamingCameraComp);
		I_REGISTER_INTERFACE(icam::IBitmapAcquisition);
		//I_REGISTER_INTERFACE(icam::IExposureConstraints);
		I_REGISTER_INTERFACE(imeas::INumericConstraints);
		I_ASSIGN(m_urlParamsIdAttrPtr, "UrlParamId", "ID used to get camera URL from the parameter set", false, "UrlParamId");
		I_ASSIGN(m_urlSuffixAttrPtr, "UrlSuffixParam", "String added to the end of url string. Must be in format val1=1&val2=2&val3=3", false, "UrlSuffixParam");
		I_ASSIGN(m_adjustParamsIdAttrPtr, "AdjustParamsId", "ID used to get brightness and contrast adjust parameters from the parameter set", false, "AdjustParams");
		I_ASSIGN(m_aoiParamsIdAttrPtr, "AoiParamId", "ID used to get AOI from parameter set", false, "AoiParamId");
		I_ASSIGN(m_framerateParamsIdAttrPtr, "FramerateParamId", "ID used to get framerate from parameter set", false, "FramerateParamId");		
		I_ASSIGN(m_defaultUrlParamCompPtr, "DefaultUrlParam", "Default camera URL used if no URL is defined", false, "DefaultUrlParam");
		I_ASSIGN(m_defaultAdjustParamsCompPtr, "DefaultAdjustParams", "Default contrast and brightness adjust parameters that will be used", false, "DefaultAdjustParams");		
	I_END_COMPONENT;

	CLibAvRtspStreamingCameraComp();

	// reimplemented (icam::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (imeas::INumericConstraints)
	virtual int GetNumericValuesCount() const;
	virtual QString GetNumericValueName(int index) const;
	virtual QString GetNumericValueDescription(int index) const;
	virtual const imeas::IUnitInfo& GetNumericValueUnitInfo(int index) const;

public Q_SLOTS:
	void frameArrived(AVFrame*, int , int, int);
	void requestReceived(QNetworkReply*);

protected:
	/**
		Reads parameters from set and starts connection if not connected yet
	*/
	void EnsureConnected(const iprm::IParamsSet* paramsPtr);

	// reimplemented (icomp::CComponentBase)
	void OnComponentCreated();
	void OnComponentDestroyed();

private:
	I_ATTR(QByteArray, m_urlParamsIdAttrPtr);
	I_ATTR(QByteArray, m_urlSuffixAttrPtr);
	I_ATTR(QByteArray, m_adjustParamsIdAttrPtr);
	I_ATTR(QByteArray, m_aoiParamsIdAttrPtr);
	I_ATTR(QByteArray, m_framerateParamsIdAttrPtr);
	I_REF(ifile::IFileNameParam, m_defaultUrlParamCompPtr);
	I_REF(imeas::ILinearAdjustParams, m_defaultAdjustParamsCompPtr);

	QUrl m_currentCameraUrl;	

	istd::TDelPtr<CLibAvRtspStreamingClient> m_streamingClientPtr;

	istd::CIndex2d m_lastImageSize;
	iimg::IBitmap* m_frameBitmapPtr;

	istd::TDelPtr<QNetworkAccessManager> m_networkAccessManagerPtr;

	QMutex *m_mutexPtr;
};

} // namespace ilibav

#endif // !ilibav_CLibAvRtspStreamingCameraComp_included


