#include "isgige/CGenicamCameraComp.h"


// Qt includes
#include <QHostAddress>

// ACF includes
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

int CGenicamCameraComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr)
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

	if (deviceInfoPtr->triggerMode == 0){
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

			double timeDiff =	imageTimestamp.GetTimeTo(*triggerTimerPtr) - *m_triggerDifferenceAttrPtr;

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
			
			if (!resultBitmapPtr->CreateBitmap(imageSize, 8)){
				SendErrorMessage(MI_CANNOT_CAPTURE, iqt::GetCString(tr("Camera %1: Cannot create 8 bit image with size %2 x %3").arg(iqt::GetQString(deviceInfoPtr->cameraId)).arg(cx).arg(cy)));
			}

			for (unsigned int i = 0; i < cy; i++){
				::memcpy(resultBitmapPtr->GetLinePtr(i), imageInfoPtr->GetRawData(i), cx);
			}
		}

		// remove one image from camera buffer
		deviceInfoPtr->devicePtr->PopImage(imageInfoPtr);	

		return TS_OK;
	} while (elapsedTime.GetElapsed() < *m_timeoutAttrPtr);

	SendInfoMessage(MI_TIMEOUT, "Cannot capture image - time out");

	return TS_INVALID;
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


// reimplemented (icam::ITriggerConstraints)

bool CGenicamCameraComp::IsTriggerSupported() const
{
	return true;
}


bool CGenicamCameraComp::IsTriggerModeSupported(int triggerMode) const
{
	return (triggerMode >= icam::ITriggerParams::TM_DEFAULT) && (triggerMode <= icam::ITriggerParams::TM_FALLING_EDGE);
}


// reimplemented (iprm::ISelectionConstraints)

int CGenicamCameraComp::GetOptionsCount() const
{
	return int(m_deviceInfos.size());
}


const istd::CString& CGenicamCameraComp::GetOptionName(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_deviceInfos.size()));

	return m_deviceInfos[index].cameraId;
}


// public slots

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
			I_ASSERT(index < int(m_deviceInfos.size()));

			return const_cast<CGenicamCameraComp::DeviceInfo*>(&m_deviceInfos[index]);
		}
	}

	return NULL;
}


CGenicamCameraComp::DeviceInfo* CGenicamCameraComp::GetDeviceByParams(const iprm::IParamsSet* paramsPtr) const
{
	if (m_urlParamsIdAttrPtr.IsValid()){
		const iprm::IFileNameParam* urlParamPtr = dynamic_cast<const iprm::IFileNameParam*>(paramsPtr->GetParameter((*m_urlParamsIdAttrPtr).ToString()));
		if (urlParamPtr != NULL){
			istd::CString urlString = urlParamPtr->GetPath();
			if (!urlString.IsEmpty()){
				return GetDeviceByUrl(urlString);
			}
		}
	}

	if (m_selectionParamIdPtr.IsValid()){
		const iprm::ISelectionParam* selectionParamPtr = dynamic_cast<const iprm::ISelectionParam*>(paramsPtr->GetParameter((*m_selectionParamIdPtr).ToString()));
		if (selectionParamPtr != NULL){
			int index = selectionParamPtr->GetSelectedOptionIndex();
			if ((index >= 0) && (index < int(m_deviceInfos.size()))){
				return const_cast<CGenicamCameraComp::DeviceInfo*>(&m_deviceInfos[index]);
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
		if ((index >= 0) && (index < int(m_deviceInfos.size()))){
			return const_cast<CGenicamCameraComp::DeviceInfo*>(&m_deviceInfos[index]);
		}
	}

	if (!m_deviceInfos.empty()){
		return const_cast<CGenicamCameraComp::DeviceInfo*>(&m_deviceInfos.front());
	}

	return NULL;
}


int CGenicamCameraComp::GetTriggerModeByParams(const iprm::IParamsSet* paramsPtr) const
{
	if (m_triggerParamsIdAttrPtr.IsValid()){
		const icam::ITriggerParams* triggerParamsPtr = dynamic_cast<const icam::ITriggerParams*>(paramsPtr->GetParameter((*m_triggerParamsIdAttrPtr).ToString()));
		if (triggerParamsPtr != NULL){
			if (!triggerParamsPtr->IsTriggerEnabled()){
				return 0;
			}

			return triggerParamsPtr->GetTriggerMode();
		}
	}

	if (m_defaultTriggerParamsCompPtr.IsValid()){
		if (!m_defaultTriggerParamsCompPtr->IsTriggerEnabled()){
			return 0;
		}

		return m_defaultTriggerParamsCompPtr->GetTriggerMode();
	}

	return 0;
}


const i2d::CRectangle* CGenicamCameraComp::GetRoiFromParams(const iprm::IParamsSet* paramsPtr) const
{
	if (m_roiParamIdPtr.IsValid()){
		const i2d::CRectangle* roiParamPtr = dynamic_cast<const i2d::CRectangle*>(paramsPtr->GetParameter((*m_roiParamIdPtr).ToString()));
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
		const icam::IExposureParams* exposureParamPtr = dynamic_cast<const icam::IExposureParams*>(paramsPtr->GetParameter((*m_exposureParamsIdAttrPtr).ToString()));
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

		if (!deviceInfoPtr->devicePtr->IsConnected()){
			deviceInfoPtr->devicePtr->SetImageBufferFrameCount(1);
			if (!deviceInfoPtr->devicePtr->Connect()){
				SendErrorMessage(MI_CANNOT_CONNECT, iqt::GetCString(tr("Camera %1: Cannot connect to camera").arg(iqt::GetQString(deviceInfoPtr->cameraId))));
				return NULL;
			}
		}

		if (SynchronizeCameraParams(paramsPtr, *deviceInfoPtr)){
			deviceInfoPtr->isInitialized = true;
			return deviceInfoPtr;
		}
	}
	else{
		SendErrorMessage(MI_DEVICE_INTERN, iqt::GetCString(tr("Cannot find camera")));
	}

	return NULL;
}


bool CGenicamCameraComp::SynchronizeCameraParams(const iprm::IParamsSet* paramsPtr, DeviceInfo& deviceInfo)
{
	bool retVal = deviceInfo.devicePtr->CommandNodeExecute("AcquisitionStop");

	retVal = deviceInfo.devicePtr->SetIntegerNodeValue("TLParamsLocked", 0) && retVal;
	retVal = deviceInfo.devicePtr->SetStringNodeValue("AcquisitionMode", "Continuous") && retVal;

	const i2d::CRectangle* roiParamsPtr = GetRoiFromParams(paramsPtr);
	if (roiParamsPtr != NULL){
		if (!deviceInfo.isInitialized || (*roiParamsPtr != deviceInfo.roi)){
			if (		!deviceInfo.devicePtr->SetIntegerNodeValue("Width", int(roiParamsPtr->GetWidth())) ||
						!deviceInfo.devicePtr->SetIntegerNodeValue("Height", int(roiParamsPtr->GetHeight())) ||
						!deviceInfo.devicePtr->SetIntegerNodeValue("OffsetX", int(roiParamsPtr->GetLeft())) ||
						!deviceInfo.devicePtr->SetIntegerNodeValue("OffsetY", int(roiParamsPtr->GetHeight()))){
				SendWarningMessage(MI_CANNOT_SET, iqt::GetCString(tr("Camera %1: cannot set ROI").arg(iqt::GetQString(deviceInfo.cameraId))));
			}

			deviceInfo.roi = *roiParamsPtr;
		}
	}

	const icam::IExposureParams* exposureParamsPtr = GetEposureTimeFromParams(paramsPtr);
	if (exposureParamsPtr != NULL){
		double exposureTime = exposureParamsPtr->GetShutterTime();
		if (exposureTime >= 0){
			if (!deviceInfo.isInitialized || (exposureTime != deviceInfo.exposureTime)){
				if (!deviceInfo.devicePtr->SetFloatNodeValue("ExposureTimeAbs", exposureTime * 1000000 + 0.5)){
					SendWarningMessage(MI_CANNOT_SET, iqt::GetCString(tr("Camera %1: cannot set exposure time").arg(iqt::GetQString(deviceInfo.cameraId))));
				}
			}
		}
	}
/*
	if (deviceInfo.devicePtr->SetStringNodeValue("GainSelector", "All")){
		retVal = deviceInfo.devicePtr->SetFloatNodeValue("GainAbs", params.GetAnalogGain()) && retVal;
	}
    
	if (deviceInfo.devicePtr->SetStringNodeValue("BlackLevelSelector", "All")){
		retVal = deviceInfo.devicePtr->SetFloatNodeValue("BlackLevelAbs", params.GetAnalogOffset()) && retVal;
	}
*/
	int triggerMode = GetTriggerModeByParams(paramsPtr);
	if (!deviceInfo.isInitialized || (triggerMode != deviceInfo.triggerMode)){
		bool setTriggerStatus = true;
		switch (triggerMode){
		case icam::ITriggerParams::TM_RISING_EDGE:
		case icam::ITriggerParams::TM_FALLING_EDGE:
			{
				setTriggerStatus = deviceInfo.devicePtr->SetStringNodeValue("TriggerMode", "On") && setTriggerStatus;
				setTriggerStatus = deviceInfo.devicePtr->SetStringNodeValue("TriggerSource", "Line2") && setTriggerStatus;

				if (triggerMode == icam::ITriggerParams::TM_FALLING_EDGE){
					setTriggerStatus = deviceInfo.devicePtr->SetStringNodeValue("TriggerActivation","FallingEdge") && setTriggerStatus;
				}
				else{   
					setTriggerStatus = deviceInfo.devicePtr->SetStringNodeValue("TriggerActivation","RisingEdge") && setTriggerStatus;
				}
			}
			break;

		case icam::ITriggerParams::TM_DEFAULT:
			setTriggerStatus = deviceInfo.devicePtr->SetStringNodeValue("TriggerMode", "On") && setTriggerStatus;
			setTriggerStatus = deviceInfo.devicePtr->SetStringNodeValue("TriggerSource", "Software") && setTriggerStatus;
			break;
		}

		if (!setTriggerStatus){
			SendWarningMessage(MI_CANNOT_SET, iqt::GetCString(tr("Camera %1: cannot set trigger mode").arg(iqt::GetQString(deviceInfo.cameraId))));
		}

		deviceInfo.triggerMode = triggerMode;
	}

	retVal = deviceInfo.devicePtr->SetIntegerNodeValue("TLParamsLocked", 1) && retVal;

	retVal = deviceInfo.devicePtr->CommandNodeExecute("AcquisitionStart") && retVal;

	return retVal;
}


// reimplemented (icomp::IComponent)

void CGenicamCameraComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (!s_cameraSigleton.gigeApi.IsValid()){
		SendCriticalMessage(MI_CANNOT_INIT, "Cannot initialize Smartek GigEVision SDK");
	}

	s_cameraSigleton.gigeApi->FindAllDevices(*m_findDevicesTimeAttrPtr);

	gige::DevicesList devicesList = s_cameraSigleton.gigeApi->GetAllDevices();
	m_deviceInfos.resize(devicesList.size());

	int devicesCount = int(devicesList.size());
	for (int i = 0; i < devicesCount; ++i){
		const gige::IDevice& devicePtr = devicesList[i];
		DeviceInfo& deviceInfo = m_deviceInfos[i];

		QHostAddress address(devicePtr->GetIpAddress());

		deviceInfo.devicePtr = devicePtr;
		deviceInfo.cameraId = iqt::GetCString(address.toString());

		m_ipAddressToIndexMap[devicePtr->GetIpAddress()] = i;
	}
}


} // namespace isgige


