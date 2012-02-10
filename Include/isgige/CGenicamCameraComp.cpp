#include "isgige/CGenicamCameraComp.h"


// STL includes
#include <cstring>

// Qt includes
#include <QHostAddress>

// ACF includes
#include "istd/TDelPtr.h"
#include "istd/TChangeNotifier.h"
#include "iprm/IParamsSet.h"
#include "iqt/iqt.h"

// ACF-Solutions include
#include "iwin/CTimer.h"


namespace isgige
{


CGenicamCameraComp::CGenicamCameraComp()
{
	QObject::connect(
				&s_cameraSigleton, SIGNAL(CameraEventLog(int, QString)),
				this, SLOT(OnCameraEventLog(int, QString)));
}


CGenicamCameraComp::~CGenicamCameraComp() 
{
	QObject::disconnect(
				&s_cameraSigleton, SIGNAL(CameraEventLog(int, QString)),
				this, SLOT(OnCameraEventLog(int, QString)));
}


// reimplemented (iproc::IBitmapAcquisition)

istd::CIndex2d CGenicamCameraComp::GetBitmapSize(const iprm::IParamsSet* paramsPtr) const
{
	const i2d::CRectangle* roiPtr = GetRoiFromParams(paramsPtr);
	if (roiPtr != NULL){
		return istd::CIndex2d(int(roiPtr->GetWidth()), int(roiPtr->GetHeight()));
	}

	DeviceInfo* deviceInfoPtr = GetDeviceByParams(paramsPtr);
	I_ASSERT((deviceInfoPtr == NULL) || deviceInfoPtr->devicePtr.IsValid());	// invalid devices cannot exist in device list

	if (deviceInfoPtr != NULL){
		INT64 width = 0;
		INT64 height = 0;
		if (		deviceInfoPtr->devicePtr->GetIntegerNodeValue("Width", width) &&
					deviceInfoPtr->devicePtr->GetIntegerNodeValue("Height", height)){
			return istd::CIndex2d(int(width), int(height));
		}
	}

	return istd::CIndex2d(-1, -1);	// unknown size
}


// reimplemented (iproc::IProcessor)

int CGenicamCameraComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* /*progressManagerPtr*/)
{
	iimg::IBitmap* resultBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if ((outputPtr != NULL) && (resultBitmapPtr == NULL)){
		SendWarningMessage(MI_BAD_PARAMS, "Output object for snap should be 'iimg::IBitmap'");

		return TS_INVALID;
	}

	DeviceInfo* deviceInfoPtr = EnsureDeviceSynchronized(paramsPtr);
	if (deviceInfoPtr == NULL){
		return TS_INVALID;
	}
	I_ASSERT(deviceInfoPtr->devicePtr.IsValid());

	if (deviceInfoPtr->triggerMode == isig::ITriggerParams::TM_SOFTWARE){
		deviceInfoPtr->devicePtr->SetStringNodeValue("LineSource", "ExposureActive");

		if (!deviceInfoPtr->devicePtr->CommandNodeExecute("TriggerSoftware")){
			SendErrorMessage(MI_CANNOT_SET, iqt::GetCString(tr("Camera %1: Cannot send software trigger").arg(iqt::GetQString(deviceInfoPtr->cameraId))));

			return false;
		}
	}

	iwin::CTimer elapsedTime;

	do{
		if (deviceInfoPtr->devicePtr->IsBufferEmpty()){
			// if image buffer is empty, wait restTime
			double restTime = *m_timeoutAttrPtr - elapsedTime.GetElapsed();
			if ((restTime <= 0.0) || !deviceInfoPtr->devicePtr->WaitForImage(restTime)){	// timeout
				break;
			}
		}

		I_ASSERT(!deviceInfoPtr->devicePtr->IsBufferEmpty());	// WaitForImage returned true, some image must be captured

		gige::IImageInfo imageInfoPtr = NULL;
		if (!deviceInfoPtr->devicePtr->GetImageInfo(&imageInfoPtr)) {
			SendErrorMessage(MI_CANNOT_CAPTURE, iqt::GetCString(tr("Camera %1: GetImageInfo failed").arg(iqt::GetQString(deviceInfoPtr->cameraId))));

			return TS_INVALID;
		}

		const isys::ITimer* triggerTimerPtr = dynamic_cast<const isys::ITimer*>(inputPtr);
		if (triggerTimerPtr != NULL){
			iwin::CTimer imageTimestamp;
			imageTimestamp.SetNativeRepresentation(imageInfoPtr->GetTimestamp());

			double timeDiff = imageTimestamp.GetTimeTo(*triggerTimerPtr) - *m_triggerDifferenceAttrPtr;

			if (timeDiff < -*m_triggerToleranceAttrPtr){	// image older than trigger
				SendWarningMessage(MI_DEVICE_INTERN, iqt::GetCString(tr("Camera %1: image dropped becouse of time difference %2 ms").arg(iqt::GetQString(deviceInfoPtr->cameraId)).arg(timeDiff * 1000)));
				deviceInfoPtr->devicePtr->PopImage(imageInfoPtr);	
				continue;	// this frame was skipped, we have to get next one
			}
			else if (timeDiff > *m_triggerToleranceAttrPtr) {	// image newer, or tcp timestamp error
				SendErrorMessage(MI_CANNOT_CAPTURE, iqt::GetCString(tr("Camera %1: Cannot capture image").arg(iqt::GetQString(deviceInfoPtr->cameraId))));
				return TS_INVALID;
			}
		}

		// image is sync to trigger, take it.
		if (resultBitmapPtr != NULL){
			istd::CChangeNotifier notifier(resultBitmapPtr);

			UINT32 cx, cy;
			imageInfoPtr->GetSize(cx, cy);
			istd::CIndex2d imageSize((int)cx, (int)cy);
			
			UINT32 pixelType;
			if (!imageInfoPtr->GetPixelType(pixelType) || (pixelType != GVSP_PIX_MONO8)){
				SendErrorMessage(MI_CANNOT_CAPTURE, iqt::GetCString(tr("Camera %1: Only 8 bit images are supported").arg(iqt::GetQString(deviceInfoPtr->cameraId))));

				return TS_INVALID;
			}
			
			if (!resultBitmapPtr->CreateBitmap(iimg::IBitmap::PF_GRAY, imageSize)){
				SendErrorMessage(MI_CANNOT_CAPTURE, iqt::GetCString(tr("Camera %1: Cannot create 8 bit image with size %2 x %3").arg(iqt::GetQString(deviceInfoPtr->cameraId)).arg(cx).arg(cy)));
			}

			for (unsigned int i = 0; i < cy; i++){
				std::memcpy(resultBitmapPtr->GetLinePtr(i), imageInfoPtr->GetRawData(i), cx);
			}
		}

		// remove one image from camera buffer
		deviceInfoPtr->devicePtr->PopImage(imageInfoPtr);	

		return TS_OK;
	} while (elapsedTime.GetElapsed() < *m_timeoutAttrPtr);

	SendInfoMessage(MI_TIMEOUT, "Cannot capture image - time out");

	return TS_INVALID;
}


void CGenicamCameraComp::InitProcessor(const iprm::IParamsSet* paramsPtr)
{
	EnsureDeviceSynchronized(paramsPtr);
}


// reimplemented (icam::IExposureConstraints)

istd::CRange CGenicamCameraComp::GetShutterTimeRange() const
{
	return istd::CRange(0.000001, 0.01);
}


istd::CRange CGenicamCameraComp::GetDelayTimeRange() const
{
	return istd::CRange(-1, -1);
}


istd::CRange CGenicamCameraComp::GetEenDelayRange() const
{
	return istd::CRange(-1, -1);
}


// reimplemented (isig::ITriggerConstraints)

bool CGenicamCameraComp::IsTriggerModeSupported(int triggerMode) const
{
	return (triggerMode >= isig::ITriggerParams::TM_CONTINUOUS) && (triggerMode <= isig::ITriggerParams::TM_FALLING_EDGE);
}


// reimplemented (iprm::ISelectionConstraints)

int CGenicamCameraComp::GetConstraintsFlags() const
{
	return SCF_NONE;
}


int CGenicamCameraComp::GetOptionsCount() const
{
	return int(m_deviceInfos.GetCount());
}


istd::CString CGenicamCameraComp::GetOptionName(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_deviceInfos.GetCount()));

	const DeviceInfo* deviceInfoPtr = m_deviceInfos.GetAt(index);
	I_ASSERT(deviceInfoPtr != NULL);

	return deviceInfoPtr->cameraId;
}


istd::CString CGenicamCameraComp::GetOptionDescription(int /*index*/) const
{
	return istd::CString();
}


std::string CGenicamCameraComp::GetOptionId(int /*index*/) const
{
	return std::string();
}


// protected methods

CGenicamCameraComp::DeviceInfo* CGenicamCameraComp::GetDeviceByUrl(const istd::CString& urlString) const
{
	QHostAddress url(iqt::GetQString(urlString));
	if (!url.isNull()){
		quint32 ipAddress = url.toIPv4Address();

		IpAddressToIndexMap::const_iterator foundIter = m_ipAddressToIndexMap.find(ipAddress);
		if (foundIter != m_ipAddressToIndexMap.end()){
			int index = foundIter->second;
			I_ASSERT(index >= 0);
			I_ASSERT(index < int(m_deviceInfos.GetCount()));

			return const_cast<DeviceInfo*>(m_deviceInfos.GetAt(index));
		}
	}

	return NULL;
}


CGenicamCameraComp::DeviceInfo* CGenicamCameraComp::GetDeviceByParams(const iprm::IParamsSet* paramsPtr) const
{
	if (m_urlParamsIdAttrPtr.IsValid()){
		const iprm::IFileNameParam* urlParamPtr = dynamic_cast<const iprm::IFileNameParam*>(paramsPtr->GetParameter(*m_urlParamsIdAttrPtr));
		if (urlParamPtr != NULL){
			istd::CString urlString = urlParamPtr->GetPath();
			if (!urlString.IsEmpty()){
				return GetDeviceByUrl(urlString);
			}
		}
	}

	if (m_selectionParamIdPtr.IsValid()){
		const iprm::ISelectionParam* selectionParamPtr = dynamic_cast<const iprm::ISelectionParam*>(paramsPtr->GetParameter(*m_selectionParamIdPtr));
		if (selectionParamPtr != NULL){
			int index = selectionParamPtr->GetSelectedOptionIndex();
			if ((index >= 0) && (index < int(m_deviceInfos.GetCount()))){
				return const_cast<DeviceInfo*>(m_deviceInfos.GetAt(index));
			}
		}
	}

	if (m_defaultUrlParamCompPtr.IsValid()){
		istd::CString urlString = m_defaultUrlParamCompPtr->GetPath();
		if (!urlString.IsEmpty()){
			return GetDeviceByUrl(urlString);
		}
	}

	if (m_defaultSelectionParamCompPtr.IsValid()){
		int index = m_defaultSelectionParamCompPtr->GetSelectedOptionIndex();
		if ((index >= 0) && (index < int(m_deviceInfos.GetCount()))){
			return const_cast<DeviceInfo*>(m_deviceInfos.GetAt(index));
		}
	}

	if (!m_deviceInfos.IsEmpty()){
		return const_cast<DeviceInfo*>(m_deviceInfos.GetAt(0));
	}

	return NULL;
}


int CGenicamCameraComp::GetTriggerModeByParams(const iprm::IParamsSet* paramsPtr) const
{
	if (m_triggerParamsIdAttrPtr.IsValid()){
		const isig::ITriggerParams* triggerParamsPtr = dynamic_cast<const isig::ITriggerParams*>(paramsPtr->GetParameter(*m_triggerParamsIdAttrPtr));
		if (triggerParamsPtr != NULL){
			return triggerParamsPtr->GetTriggerMode();
		}
	}

	if (m_defaultTriggerParamsCompPtr.IsValid()){
		return m_defaultTriggerParamsCompPtr->GetTriggerMode();
	}

	return 0;
}


const iprm::ILinearAdjustParams* CGenicamCameraComp::GetAdjustFromParams(const iprm::IParamsSet* paramsPtr) const
{
	if (m_adjustParamsIdAttrPtr.IsValid()){
		const iprm::ILinearAdjustParams* adjustParamsPtr = dynamic_cast<const iprm::ILinearAdjustParams*>(paramsPtr->GetParameter(*m_adjustParamsIdAttrPtr));
		if (adjustParamsPtr != NULL){
			return adjustParamsPtr;
		}
	}

	if (m_defaultAdjustParamsCompPtr.IsValid()){
		return m_defaultAdjustParamsCompPtr.GetPtr();
	}

	return NULL;
}


const i2d::CRectangle* CGenicamCameraComp::GetRoiFromParams(const iprm::IParamsSet* paramsPtr) const
{
	if (m_roiParamIdPtr.IsValid()){
		const i2d::CRectangle* roiParamPtr = dynamic_cast<const i2d::CRectangle*>(paramsPtr->GetParameter(*m_roiParamIdPtr));
		if (roiParamPtr != NULL){
			return roiParamPtr;
		}
	}

	if (m_defaultRoiParamCompPtr.IsValid()){
		return m_defaultRoiParamCompPtr.GetPtr();
	}

	return NULL;
}


const icam::IExposureParams* CGenicamCameraComp::GetEposureTimeFromParams(const iprm::IParamsSet* paramsPtr) const
{
	if (m_exposureParamsIdAttrPtr.IsValid()){
		const icam::IExposureParams* exposureParamPtr = dynamic_cast<const icam::IExposureParams*>(paramsPtr->GetParameter(*m_exposureParamsIdAttrPtr));
		if (exposureParamPtr != NULL){
			return exposureParamPtr;
		}
	}

	if (m_defaultExposureParamsCompPtr.IsValid()){
		return m_defaultExposureParamsCompPtr.GetPtr();
	}

	return NULL;
}


CGenicamCameraComp::DeviceInfo* CGenicamCameraComp::EnsureDeviceSynchronized(const iprm::IParamsSet* paramsPtr)
{
	DeviceInfo* deviceInfoPtr = GetDeviceByParams(paramsPtr);
	if (deviceInfoPtr != NULL){
		I_ASSERT(deviceInfoPtr->devicePtr.IsValid());

		if (!SynchronizeCameraParams(paramsPtr, *deviceInfoPtr)){
			return NULL;
		}

		return deviceInfoPtr;
	}
	else{
		SendErrorMessage(MI_DEVICE_INTERN, iqt::GetCString(tr("Cannot find camera")));
	}

	return NULL;
}


bool CGenicamCameraComp::SynchronizeCameraParams(const iprm::IParamsSet* paramsPtr, DeviceInfo& deviceInfo)
{
	if (!deviceInfo.EnsureConnected()){
		return NULL;
	}

	const i2d::CRectangle* roiParamsPtr = GetRoiFromParams(paramsPtr);
	const icam::IExposureParams* exposureParamsPtr = GetEposureTimeFromParams(paramsPtr);
	const iprm::ILinearAdjustParams* adjustParamsPtr = GetAdjustFromParams(paramsPtr);
	int triggerMode = GetTriggerModeByParams(paramsPtr);

	bool needLock = false;
	if (deviceInfo.isStarted){
		if (roiParamsPtr != NULL){
			if (*roiParamsPtr != deviceInfo.roi){
				needLock = true;
			}
		}

		if (exposureParamsPtr != NULL){
			double exposureTime = exposureParamsPtr->GetShutterTime();
			if (exposureTime >= 0){
				if (exposureTime != deviceInfo.exposureTime){
					needLock = true;
				}
			}
		}

		if (adjustParamsPtr != NULL){
			double brightness = adjustParamsPtr->GetOffsetFactor();
			double contrast = adjustParamsPtr->GetScaleFactor();
			if ((brightness != deviceInfo.brightness) || (contrast != deviceInfo.contrast)){
				needLock = true;
			}
		}

		if (triggerMode != deviceInfo.triggerMode){
			needLock = true;
		}
	}
	else{
		needLock = true;
	}

	bool retVal = true;
	if (needLock){
		if (!deviceInfo.EnsureStopped()){
			return false;
		}
		retVal = deviceInfo.devicePtr->SetIntegerNodeValue("TLParamsLocked", 0) && retVal;

		if (!deviceInfo.devicePtr->SetStringNodeValue("AcquisitionMode", "Continuous")){
			SendErrorMessage(MI_CANNOT_SET, iqt::GetCString(tr("Camera %1: Cannot set initial values").arg(iqt::GetQString(deviceInfo.cameraId))));
		}

		if (roiParamsPtr != NULL){
			if (!deviceInfo.isStarted || (*roiParamsPtr != deviceInfo.roi)){
				if (		!deviceInfo.devicePtr->SetIntegerNodeValue("Width", int(roiParamsPtr->GetWidth())) ||
							!deviceInfo.devicePtr->SetIntegerNodeValue("Height", int(roiParamsPtr->GetHeight())) ||
							!deviceInfo.devicePtr->SetIntegerNodeValue("OffsetX", int(roiParamsPtr->GetLeft())) ||
							!deviceInfo.devicePtr->SetIntegerNodeValue("OffsetY", int(roiParamsPtr->GetHeight()))){
					SendWarningMessage(MI_CANNOT_SET, iqt::GetCString(tr("Camera %1: cannot set ROI").arg(iqt::GetQString(deviceInfo.cameraId))));
				}

				deviceInfo.roi = *roiParamsPtr;
			}
		}

		if (exposureParamsPtr != NULL){
			double exposureTime = exposureParamsPtr->GetShutterTime();
			if (exposureTime >= 0){
				if (!deviceInfo.isStarted || (exposureTime != deviceInfo.exposureTime)){
					if (!deviceInfo.devicePtr->SetFloatNodeValue("ExposureTimeAbs", exposureTime * 1000000 + 0.5)){
						SendWarningMessage(MI_CANNOT_SET, iqt::GetCString(tr("Camera %1: cannot set exposure time").arg(iqt::GetQString(deviceInfo.cameraId))));
					}

					deviceInfo.exposureTime = exposureTime;
				}
			}
		}

		if (adjustParamsPtr != NULL){
			double brightness = adjustParamsPtr->GetOffsetFactor();
			double contrast = adjustParamsPtr->GetScaleFactor();
			if (!deviceInfo.isStarted || (brightness != deviceInfo.brightness) || (contrast != deviceInfo.contrast)){
				if (deviceInfo.devicePtr->SetStringNodeValue("GainSelector", "All")){
					retVal = deviceInfo.devicePtr->SetFloatNodeValue("GainAbs", 0) && retVal;
				}
			    
				if (deviceInfo.devicePtr->SetStringNodeValue("BlackLevelSelector", "All")){
					retVal = deviceInfo.devicePtr->SetFloatNodeValue("BlackLevelAbs", 0) && retVal;
				}
			}
		}

		if (!deviceInfo.isStarted || (triggerMode != deviceInfo.triggerMode)){
			bool setTriggerStatus = true;

			switch (triggerMode){
			case isig::ITriggerParams::TM_CONTINUOUS:
				setTriggerStatus = deviceInfo.devicePtr->SetStringNodeValue("TriggerMode", "Off") && setTriggerStatus;
				break;

			case isig::ITriggerParams::TM_RISING_EDGE:
			case isig::ITriggerParams::TM_FALLING_EDGE:
				{
					setTriggerStatus = deviceInfo.devicePtr->SetStringNodeValue("TriggerSource", "Line2") && setTriggerStatus;

					if (triggerMode == isig::ITriggerParams::TM_FALLING_EDGE){
						setTriggerStatus = deviceInfo.devicePtr->SetStringNodeValue("TriggerActivation","FallingEdge") && setTriggerStatus;
					}
					else{   
						setTriggerStatus = deviceInfo.devicePtr->SetStringNodeValue("TriggerActivation","RisingEdge") && setTriggerStatus;
					}
					setTriggerStatus = deviceInfo.devicePtr->SetStringNodeValue("TriggerMode", "On") && setTriggerStatus;
				}
				break;

			case isig::ITriggerParams::TM_SOFTWARE:
				setTriggerStatus = deviceInfo.devicePtr->SetStringNodeValue("TriggerSource", "Software") && setTriggerStatus;
				setTriggerStatus = deviceInfo.devicePtr->SetStringNodeValue("TriggerMode", "On") && setTriggerStatus;
				break;

			default:
				setTriggerStatus = false;
			}

			if (!setTriggerStatus){
				SendWarningMessage(MI_CANNOT_SET, iqt::GetCString(tr("Camera %1: cannot set trigger mode").arg(iqt::GetQString(deviceInfo.cameraId))));
			}

			deviceInfo.triggerMode = triggerMode;
		}

		retVal = deviceInfo.devicePtr->SetStringNodeValue("LineSelector", "Line1") && retVal;
		retVal = deviceInfo.devicePtr->SetStringNodeValue("LineSource", "ExposureActive") && retVal;

		retVal = deviceInfo.devicePtr->SetIntegerNodeValue("TLParamsLocked", 1) && retVal;

		if (!deviceInfo.EnsureStarted()){
			return false;
		}
	}

	return retVal;
}


// reimplemented (icomp::CComponentBase)

void CGenicamCameraComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (!s_cameraSigleton.gigeApi.IsValid()){
		SendCriticalMessage(MI_CANNOT_INIT, "Cannot initialize Smartek GigEVision SDK");
	}

	s_cameraSigleton.gigeApi->FindAllDevices(*m_findDevicesTimeAttrPtr);

	gige::DevicesList devicesList = s_cameraSigleton.gigeApi->GetAllDevices();

	int devicesCount = int(devicesList.size());
	for (int i = 0; i < devicesCount; ++i){
		gige::IDevice& devicePtr = devicesList[i];
		istd::TDelPtr<DeviceInfo> deviceInfoPtr(new DeviceInfo(this));

		QHostAddress address(devicePtr->GetIpAddress());

		deviceInfoPtr->devicePtr = devicePtr;
		deviceInfoPtr->cameraId = iqt::GetCString(address.toString());

		m_ipAddressToIndexMap[devicePtr->GetIpAddress()] = i;

		devicePtr->SetImageBufferFrameCount(10);

		if (*m_connectOnStartAttrPtr){
			deviceInfoPtr->EnsureConnected();
		}

		m_deviceInfos.PushBack(deviceInfoPtr.PopPtr());
	}
}


void CGenicamCameraComp::OnComponentDestroyed()
{
	m_deviceInfos.Reset();

	BaseClass::OnComponentDestroyed();
}


// protected slots

void CGenicamCameraComp::OnCameraEventLog(int type, QString message)
{
	istd::ILogger::MessageCategory category = istd::ILogger::MC_CRITICAL;
	switch (type){
	case gige_EM_TYPE_INFO:
		category = istd::ILogger::MC_INFO;
		break;

	case gige_EM_TYPE_WARNING:
		category = istd::ILogger::MC_WARNING;
		break;

	case gige_EM_TYPE_ERROR:
		category = istd::ILogger::MC_ERROR;
		break;
	}

	SendLogMessage(category, MI_DEVICE_INTERN, iqt::GetCString(message), "");
}


// public methods of embedded class DeviceInfo

CGenicamCameraComp::DeviceInfo::DeviceInfo(CGenicamCameraComp* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);

	exposureTime = -10000;
	brightness = -10000;
	contrast = -10000;
	triggerMode = -1;

	isStarted = false;
}


CGenicamCameraComp::DeviceInfo::~DeviceInfo()
{
	EnsureStopped();
}


bool CGenicamCameraComp::DeviceInfo::EnsureConnected()
{
	if (!devicePtr.IsValid()){
		return false;
	}

	if (!devicePtr->IsConnected()){
		isStarted = false;

		if (!devicePtr->Connect()){
			m_parent.SendErrorMessage(MI_CANNOT_CONNECT, iqt::GetCString(tr("Camera %1: Cannot connect to camera").arg(iqt::GetQString(cameraId))));

			return false;
		}
	}

	return true;
}


bool CGenicamCameraComp::DeviceInfo::EnsureStarted()
{
	if (!devicePtr.IsValid()){
		return false;
	}

	if (!isStarted && !devicePtr->CommandNodeExecute("AcquisitionStart")){
		m_parent.SendWarningMessage(MI_CANNOT_SET, iqt::GetCString(tr("Camera %1: cannot start acquisition").arg(iqt::GetQString(cameraId))));

		return false;
	}

	isStarted = true;

	return true;
}


bool CGenicamCameraComp::DeviceInfo::EnsureStopped()
{
	if (!devicePtr.IsValid()){
		return false;
	}

	if (isStarted && !devicePtr->CommandNodeExecute("AcquisitionStop")){
		m_parent.SendInfoMessage(MI_CANNOT_SET, iqt::GetCString(tr("Camera %1: cannot stop acquisition").arg(iqt::GetQString(cameraId))));

		return false;
	}

	isStarted = false;

	return true;
}


} // namespace isgige


