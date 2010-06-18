#ifndef isgige_CGenicamCameraComp_included
#define isgige_CGenicamCameraComp_included


// GigEVision includes
#include "gige_cpp/IGigEVisionAPI.h"

// STL includes
#include <vector>
#include <map>

// ACF includes
#include "i2d/CRectangle.h"
#include "icomp/CComponentBase.h"
#include "ibase/TLoggerCompWrap.h"
#include "iprm/ISelectionConstraints.h"
#include "iprm/ISelectionParam.h"
#include "iprm/IFileNameParam.h"
#include "iproc/IBitmapAcquisition.h"
#include "iproc/TSyncProcessorWrap.h"

#include "icam/IExposureConstraints.h"
#include "icam/ITriggerConstraints.h"
#include "icam/IExposureParams.h"
#include "icam/ITriggerParams.h"

#include "isgige/CGenicamCameraSingleton.h"


namespace isgige
{


/**
	Camera accessed over interface Genicam.
*/
class CGenicamCameraComp:
			public QObject,
			public ibase::CLoggerComponentBase,
			virtual public icam::IExposureConstraints,
			virtual public icam::ITriggerConstraints,
			virtual public iprm::ISelectionConstraints,
			virtual public iproc::TSyncProcessorWrap<iproc::IBitmapAcquisition>
{
	Q_OBJECT

public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CGenicamCameraComp);
		I_REGISTER_INTERFACE(iproc::IBitmapAcquisition);
		I_REGISTER_INTERFACE(icam::IExposureConstraints);
		I_REGISTER_INTERFACE(icam::ITriggerConstraints);
		I_REGISTER_INTERFACE(iprm::ISelectionConstraints);
		I_ASSIGN(m_defaultUrlParamCompPtr, "DefaultUrlParam", "Default camera URL used if no URL is defined", false, "DefaultUrlParam");
		I_ASSIGN(m_defaultSelectionParamCompPtr, "DefaultSelectionParam", "Default camera selection params used if no selection nor URL is found", false, "DefaultSelectionParam");
		I_ASSIGN(m_defaultExposureParamsCompPtr, "DefaultExposureParams", "Default exposure parameters will be used if no parameters are found", false, "DefaultExposureParams");
		I_ASSIGN(m_defaultTriggerParamsCompPtr, "DefaultTriggerParams", "Default trigger parameters that will be used", false, "DefaultTriggerParams");
//		I_ASSIGN(m_defaultRoiParamCompPtr, "DefaultRoiParam", "Default region of interest (ROI) that will be used if no defined", false, "DefaultImageRegionParams");
		I_ASSIGN(m_urlParamsIdAttrPtr, "UrlParamId", "ID used to get camera URL from the parameter set", false, "UrlParamId");
		I_ASSIGN(m_selectionParamIdPtr, "SelectionParamId", "ID used to get selected camera index from the parameter set", false, "UrlParamId");
		I_ASSIGN(m_exposureParamsIdAttrPtr, "ExposureParamsId", "ID used to get exposure parameters from the parameter set", false, "ExposureParams");
		I_ASSIGN(m_triggerParamsIdAttrPtr, "TriggerParamsId", "ID used to get trigger parameters from the parameter set", false, "TriggerParams");
		I_ASSIGN(m_roiParamIdPtr, "RoiParamId", "ID used to get region of interest (ROI) from the parameter set", false, "DefaultImageRegionParams");
		I_ASSIGN(m_timeoutAttrPtr, "Timeout", "Acquisition timeout", true, 5.0);
		I_ASSIGN(m_findDevicesTimeAttrPtr, "FindDevicesTime", "Time to search camera devices on start", true, 1.0);
		I_ASSIGN(m_triggerDifferenceAttrPtr, "TriggerDifference", "Time difference between trigger signal and image time stamp used for synchronized trigger (in seconds)", true, 0.01);
		I_ASSIGN(m_triggerToleranceAttrPtr, "TriggerTolerance", "Tolerance of trigger time difference used for synchronized trigger (in seconds)", true, 0.02);
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

	// reimplemented (iproc::IBitmapAcquisition)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr);

	// reimplemented (iproc::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented (iproc::IProcessor)
	virtual void InitProcessor(const iprm::IParamsSet* paramsPtr);

	// reimplemented (icam::IExposureConstraints)
	virtual istd::CRange GetShutterTimeRange() const;
	virtual istd::CRange GetDelayTimeRange() const;
	virtual istd::CRange GetEenDelayRange() const;

	// reimplemented (icam::ITriggerConstraints)
	virtual bool IsTriggerSupported() const;
	virtual bool IsTriggerModeSupported(int triggerMode) const;

	// reimplemented (iprm::ISelectionConstraints)
	virtual int GetOptionsCount() const;
	virtual const istd::CString& GetOptionName(int index) const;

protected:
	struct DeviceInfo
	{
		DeviceInfo()
		{
			exposureTime = -10000;
			brightness = -10000;
			contrast = -10000;
			triggerMode = -1;

			isInitialized = false;
		}

		double exposureTime;
		double brightness;
		double contrast;
		int triggerMode;
		i2d::CRectangle roi;

		gige::IDevice devicePtr;

		istd::CString cameraId;

		bool isInitialized;
	};

	DeviceInfo* GetDeviceByUrl(const istd::CString& urlString) const;
	DeviceInfo* GetDeviceByParams(const iprm::IParamsSet* paramsPtr) const;

	int GetTriggerModeByParams(const iprm::IParamsSet* paramsPtr) const;
	const i2d::CRectangle* GetRoiFromParams(const iprm::IParamsSet* paramsPtr) const;
	const icam::IExposureParams* GetEposureTimeFromParams(const iprm::IParamsSet* paramsPtr) const;

	DeviceInfo* EnsureDeviceSynchronized(const iprm::IParamsSet* paramsPtr);
	bool SynchronizeCameraParams(const iprm::IParamsSet* paramsPtr, DeviceInfo& deviceInfo);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

protected slots:
	void OnCameraEventLog(int type, QString message);

private:
	I_REF(iprm::IFileNameParam, m_defaultUrlParamCompPtr);
	I_REF(iprm::ISelectionParam, m_defaultSelectionParamCompPtr);
	I_REF(icam::IExposureParams, m_defaultExposureParamsCompPtr);
	I_REF(icam::ITriggerParams, m_defaultTriggerParamsCompPtr);
	I_REF(i2d::CRectangle, m_defaultRoiParamCompPtr);
	I_ATTR(istd::CString, m_urlParamsIdAttrPtr);
	I_ATTR(istd::CString, m_selectionParamIdPtr);
	I_ATTR(istd::CString, m_exposureParamsIdAttrPtr);
	I_ATTR(istd::CString, m_triggerParamsIdAttrPtr);
	I_ATTR(istd::CString, m_roiParamIdPtr);

	I_ATTR(double, m_timeoutAttrPtr);
	I_ATTR(double, m_findDevicesTimeAttrPtr);

	I_ATTR(double, m_triggerToleranceAttrPtr);
	I_ATTR(double, m_triggerDifferenceAttrPtr);

	bool m_isCameraValid;

	typedef std::vector<DeviceInfo> DeviceInfos;
	DeviceInfos m_deviceInfos;
	typedef std::map<I_DWORD, int> IpAddressToIndexMap;
	IpAddressToIndexMap m_ipAddressToIndexMap;

	// static attributes
	CGenicamCameraSingleton s_cameraSigleton;
};


} // namespace isgige


#endif // !isgige_CGenicamCameraComp_included


