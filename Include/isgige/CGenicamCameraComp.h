#ifndef isgige_CGenicamCameraComp_included
#define isgige_CGenicamCameraComp_included


// GigEVision includes
#include "gige_cpp/IGigEVisionAPI.h"

// Qt includes
#include <QtCore/QMap>

// ACF includes
#include "istd/TPointerVector.h"
#include "i2d/CRectangle.h"
#include "ibase/TLoggerCompWrap.h"
#include "iprm/ISelectionConstraints.h"
#include "iprm/ISelectionParam.h"
#include "iprm/IFileNameParam.h"
#include "iprm/ILinearAdjustParams.h"
#include "iproc/TSyncProcessorWrap.h"

// ACF-Solutions includes
#include "icam/IBitmapAcquisition.h"
#include "icam/IExposureConstraints.h"
#include "icam/IExposureParams.h"
#include "isig/ITriggerParams.h"
#include "isig/ITriggerConstraints.h"

// IACF includes
#include "isgige/CGenicamCameraSingleton.h"


namespace isgige
{


class CGenicamCameraCompBase: public ibase::CLoggerComponentBase
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CGenicamCameraCompBase);
		I_ASSIGN(m_defaultUrlParamCompPtr, "DefaultUrlParam", "Default camera URL used if no URL is defined", false, "DefaultUrlParam");
		I_ASSIGN(m_defaultSelectionParamCompPtr, "DefaultSelectionParam", "Default camera selection params used if no selection nor URL is found", false, "DefaultSelectionParam");
		I_ASSIGN(m_defaultExposureParamsCompPtr, "DefaultExposureParams", "Default exposure parameters will be used if no parameters are found", false, "DefaultExposureParams");
		I_ASSIGN(m_defaultTriggerParamsCompPtr, "DefaultTriggerParams", "Default trigger parameters that will be used", false, "DefaultTriggerParams");
		I_ASSIGN(m_defaultAdjustParamsCompPtr, "DefaultAdjustParams", "Default contrast and brightness adjust parameters that will be used", false, "DefaultAdjustParams");
		I_ASSIGN(m_defaultRoiParamCompPtr, "DefaultRoiParam", "Default region of interest (ROI) that will be used if no defined", false, "DefaultImageRegionParams");
	I_END_COMPONENT;

protected:
	I_REF(iprm::IFileNameParam, m_defaultUrlParamCompPtr);
	I_REF(iprm::ISelectionParam, m_defaultSelectionParamCompPtr);
	I_REF(icam::IExposureParams, m_defaultExposureParamsCompPtr);
	I_REF(isig::ITriggerParams, m_defaultTriggerParamsCompPtr);
	I_REF(iprm::ILinearAdjustParams, m_defaultAdjustParamsCompPtr);
	I_REF(i2d::CRectangle, m_defaultRoiParamCompPtr);
};


/**
	Camera accessed over interface Genicam.
*/
class CGenicamCameraComp:
			public QObject,
			public CGenicamCameraCompBase,
			public iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>,
			virtual public icam::IExposureConstraints,
			virtual public isig::ITriggerConstraints,
			virtual public iprm::ISelectionConstraints
{
	Q_OBJECT

public:
	typedef CGenicamCameraCompBase BaseClass;

	I_BEGIN_COMPONENT(CGenicamCameraComp);
		I_REGISTER_INTERFACE(icam::IBitmapAcquisition);
		I_REGISTER_INTERFACE(icam::IExposureConstraints);
		I_REGISTER_INTERFACE(isig::ITriggerConstraints);
		I_REGISTER_INTERFACE(iprm::ISelectionConstraints);
		I_ASSIGN(m_urlParamsIdAttrPtr, "UrlParamId", "ID used to get camera URL from the parameter set", false, "UrlParamId");
		I_ASSIGN(m_selectionParamIdPtr, "SelectionParamId", "ID used to get selected camera index from the parameter set", false, "UrlParamId");
		I_ASSIGN(m_exposureParamsIdAttrPtr, "ExposureParamsId", "ID used to get exposure parameters from the parameter set", false, "ExposureParams");
		I_ASSIGN(m_triggerParamsIdAttrPtr, "TriggerParamsId", "ID used to get trigger parameters from the parameter set", false, "TriggerParams");
		I_ASSIGN(m_adjustParamsIdAttrPtr, "AdjustParamsId", "ID used to get brightness and contrast adjust parameters from the parameter set", false, "AdjustParams");
		I_ASSIGN(m_roiParamIdPtr, "RoiParamId", "ID used to get region of interest (ROI) from the parameter set", false, "DefaultImageRegionParams");
		I_ASSIGN(m_timeoutAttrPtr, "Timeout", "Acquisition timeout", true, 5.0);
		I_ASSIGN(m_findDevicesTimeAttrPtr, "FindDevicesTime", "Time to search camera devices on start", true, 1.0);
		I_ASSIGN(m_triggerDifferenceAttrPtr, "TriggerDifference", "Time difference between trigger signal and image time stamp used for synchronized trigger (in seconds)", true, 0.01);
		I_ASSIGN(m_triggerToleranceAttrPtr, "TriggerTolerance", "Tolerance of trigger time difference used for synchronized trigger (in seconds)", true, 0.02);
		I_ASSIGN(m_connectOnStartAttrPtr, "ConnectOnStart", "If true connection on start will be done, if false only connection on demand is done", true, true);
	I_END_COMPONENT;

	enum MessageId
	{
		MI_CANNOT_INIT = 0x2ef3310,
		MI_CANNOT_SET,
		MI_TIMEOUT,
		MI_CANNOT_CONNECT,
		MI_CANNOT_CAPTURE,
		MI_DEVICE_INTERN
	};

	CGenicamCameraComp();
	virtual ~CGenicamCameraComp();

	// reimplemented (icam::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);
	virtual void InitProcessor(const iprm::IParamsSet* paramsPtr);

	// reimplemented (icam::IExposureConstraints)
	virtual istd::CRange GetShutterTimeRange() const;
	virtual istd::CRange GetDelayTimeRange() const;
	virtual istd::CRange GetEenDelayRange() const;

	// reimplemented (isig::ITriggerConstraints)
	virtual bool IsTriggerModeSupported(int triggerMode) const;

	// reimplemented (iprm::ISelectionConstraints)
	virtual int GetConstraintsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;

protected:
	struct DeviceInfo
	{
		DeviceInfo(CGenicamCameraComp* parentPtr);
		~DeviceInfo();

		bool EnsureConnected();

		bool EnsureStarted();
		bool EnsureStopped();

		double exposureTime;
		double brightness;
		double contrast;
		int triggerMode;
		i2d::CRectangle roi;

		gige::IDevice devicePtr;

		QString cameraId;

		bool isStarted;

	private:
		CGenicamCameraComp& m_parent;
	};

	DeviceInfo* GetDeviceByUrl(const QString& urlString) const;
	DeviceInfo* GetDeviceByParams(const iprm::IParamsSet* paramsPtr) const;

	int GetTriggerModeByParams(const iprm::IParamsSet* paramsPtr) const;
	const iprm::ILinearAdjustParams* GetAdjustFromParams(const iprm::IParamsSet* paramsPtr) const;
	const i2d::CRectangle* GetRoiFromParams(const iprm::IParamsSet* paramsPtr) const;
	const icam::IExposureParams* GetEposureTimeFromParams(const iprm::IParamsSet* paramsPtr) const;

	DeviceInfo* EnsureDeviceSynchronized(const iprm::IParamsSet* paramsPtr);
	bool SynchronizeCameraParams(const iprm::IParamsSet* paramsPtr, DeviceInfo& deviceInfo);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected Q_SLOTS:
	void OnCameraEventLog(int type, QString message);

private:
	I_ATTR(QByteArray, m_urlParamsIdAttrPtr);
	I_ATTR(QByteArray, m_selectionParamIdPtr);
	I_ATTR(QByteArray, m_exposureParamsIdAttrPtr);
	I_ATTR(QByteArray, m_triggerParamsIdAttrPtr);
	I_ATTR(QByteArray, m_adjustParamsIdAttrPtr);
	I_ATTR(QByteArray, m_roiParamIdPtr);
	I_ATTR(double, m_timeoutAttrPtr);
	I_ATTR(double, m_findDevicesTimeAttrPtr);
	I_ATTR(double, m_triggerToleranceAttrPtr);
	I_ATTR(double, m_triggerDifferenceAttrPtr);
	I_ATTR(bool, m_connectOnStartAttrPtr);

	typedef istd::TPointerVector<DeviceInfo> DeviceInfos;
	DeviceInfos m_deviceInfos;
	typedef QMap<quint32, int> IpAddressToIndexMap;
	IpAddressToIndexMap m_ipAddressToIndexMap;

	// static attributes
	CGenicamCameraSingleton s_cameraSigleton;
};


} // namespace isgige


#endif // !isgige_CGenicamCameraComp_included


