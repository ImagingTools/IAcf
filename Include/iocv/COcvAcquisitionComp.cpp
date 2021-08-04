#include <iocv/COcvAcquisitionComp.h>


// ACF includes 
#include <istd/CChangeNotifier.h>
#include <iprm/IParamsSet.h>
#include <iprm/ISelectionParam.h>
#include <iprm/TParamsPtr.h>
#include <iocv/COcvImage.h>


namespace iocv
{


COcvAcquisitionComp::COcvAcquisitionComp()
{
	m_supportedCameraDriversMap[cv::CAP_VFW] = "Video for Windows";
	m_supportedCameraDriversMap[cv::CAP_IEEE1394] = "FireWire (IEEE 1394)";
	m_supportedCameraDriversMap[cv::CAP_QT] = "Quick Time";
	m_supportedCameraDriversMap[cv::CAP_UNICAP] = "Unicap";
	m_supportedCameraDriversMap[cv::CAP_DSHOW] = "DirectShow";
	m_supportedCameraDriversMap[cv::CAP_MSMF] = "Microsoft Media Foundation";
	m_supportedCameraDriversMap[cv::CAP_PVAPI] = "Prosilica GigE SDK";
	m_supportedCameraDriversMap[cv::CAP_OPENNI] = "OpenNI (Kinect)";
	m_supportedCameraDriversMap[cv::CAP_ANDROID] = "Android Capture";
	m_supportedCameraDriversMap[cv::CAP_XIAPI] = "XIMEA";
	m_supportedCameraDriversMap[cv::CAP_AVFOUNDATION] = "AVFoundation Framework for iOS";
	m_supportedCameraDriversMap[cv::CAP_GIGANETIX] = "Smartek Giganetix";
	m_supportedCameraDriversMap[cv::CAP_INTELPERC] = "Intel Perceptual Computing";
	m_supportedCameraDriversMap[cv::CAP_ARAVIS] = "Aravis GigE";
	m_supportedCameraDriversMap[cv::CAP_OPENNI2] = "Open NI2";
	m_supportedCameraDriversMap[cv::CAP_FFMPEG] = "FFMPEG";

	m_deviceInfoList.SetParent(this);
}


// reimplemented (iproc::IProcessor)

int COcvAcquisitionComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* /*inputPtr*/,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	if (m_deviceList.IsEmpty()){
		return TS_INVALID;
	}

	iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (bitmapPtr == NULL){
		return TS_INVALID;
	}

	istd::CChangeNotifier notifier(bitmapPtr);

	CameraDevice* selectedDevicePtr = GetSelectedCameraDevice(paramsPtr);
	if (selectedDevicePtr != NULL){
		cv::Mat cameraImage;

		if (selectedDevicePtr->retrieve(cameraImage)){
			if (iocv::COcvImage::ConvertToBitmap(cameraImage, *bitmapPtr)){
				return TS_OK;
			}
		}
	}

	return TS_INVALID;
}


// reimplemented (icam::IBitmapAcquisition)

istd::CIndex2d COcvAcquisitionComp::GetBitmapSize(const iprm::IParamsSet* paramsPtr) const
{
	CameraDevice* selectedDevicePtr = GetSelectedCameraDevice(paramsPtr);
	if (selectedDevicePtr != NULL){
		int imageWidth = (int)selectedDevicePtr->get(cv::CAP_PROP_FRAME_WIDTH);
		int imageHeight = (int)(int)selectedDevicePtr->get(cv::CAP_PROP_FRAME_HEIGHT);
	
		return istd::CIndex2d(imageWidth, imageHeight);
	}

	return istd::CIndex2d(-1, -1);
}


// protected methods

// reimplemented (icomp::CComponentBase)

void COcvAcquisitionComp::OnComponentCreated()
{
	EnumerateCameraDevices();

	BaseClass::OnComponentCreated();
}


void COcvAcquisitionComp::OnComponentDestroyed()
{
	m_deviceList.Reset();

	BaseClass::OnComponentDestroyed();
}


// private methods

void COcvAcquisitionComp::EnumerateCameraDevices()
{
	istd::TDelPtr<CameraDevice> devicePtr = new CameraDevice(0, "Auto-detected");
	if (devicePtr.IsValid() && devicePtr->isOpened()){
		m_deviceList.PushBack(devicePtr.PopPtr());
	}

	for (		CameraDriversMap::ConstIterator cameraDriverIter = m_supportedCameraDriversMap.constBegin();
				cameraDriverIter != m_supportedCameraDriversMap.constEnd();
				++cameraDriverIter) {
		int cameraDriverId = cameraDriverIter.key();

		devicePtr = new CameraDevice(cameraDriverId, cameraDriverIter.value());
		if (devicePtr.IsValid() && devicePtr->isOpened()) {
			m_deviceList.PushBack(devicePtr.PopPtr());
		}
	}
}


COcvAcquisitionComp::CameraDevice* COcvAcquisitionComp::GetSelectedCameraDevice(const iprm::IParamsSet* paramsPtr) const
{
	if ((paramsPtr == NULL) && !m_deviceList.IsEmpty()){
		return m_deviceList.GetElementAt(0);
	}

	iprm::TParamsPtr<iprm::ISelectionParam> cameraIndexParamPtr(paramsPtr, *m_cameraIndexAttrPtr);
	if (cameraIndexParamPtr.IsValid()){
		int cameraIndex = cameraIndexParamPtr->GetSelectedOptionIndex();
		if ((cameraIndex >= 0) && (cameraIndex < m_deviceList.GetCount())){
			return m_deviceList.GetElementAt(cameraIndex);
		}
	}

	return NULL;
}


// public methods of the embedded class CameraDevice

COcvAcquisitionComp::CameraDevice::CameraDevice(int cameraDriverId, const QString& deviceName)
	:BaseClass(cameraDriverId),
	m_deviceName(deviceName)
{
}


COcvAcquisitionComp::CameraDevice::CameraDevice(const QString& streamUrl, const QString& deviceName)
	:BaseClass(),
	m_deviceName(deviceName)
{
	BaseClass::open(streamUrl.toStdString());
}


QString COcvAcquisitionComp::CameraDevice::GetDeviceName() const
{
	return m_deviceName;
}


// public methods of the embedded class DeviceInfoList

COcvAcquisitionComp::DeviceInfoList::DeviceInfoList()
	:m_parentPtr(NULL)
{
}


void COcvAcquisitionComp::DeviceInfoList::SetParent(COcvAcquisitionComp* parentPtr)
{
	m_parentPtr = parentPtr;
}


// reimplemented (iprm::IOptionsList)

int COcvAcquisitionComp::DeviceInfoList::GetOptionsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID;
}


int COcvAcquisitionComp::DeviceInfoList::GetOptionsCount() const
{
	Q_ASSERT(m_parentPtr != NULL);

	return m_parentPtr->m_deviceList.GetCount();
}


QString COcvAcquisitionComp::DeviceInfoList::GetOptionName(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index >= 0);

	if (index < m_parentPtr->m_deviceList.GetCount()){
		return m_parentPtr->m_deviceList.GetAt(index)->GetDeviceName();
	}
	else{
		return "Unnamed";
	}
}


QString COcvAcquisitionComp::DeviceInfoList::GetOptionDescription(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index >= 0);
	Q_UNUSED(index);

	return QString();
}


QByteArray COcvAcquisitionComp::DeviceInfoList::GetOptionId(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index >= 0);

	return QString("Camera-%1").arg(index + 1).toUtf8();
}


bool COcvAcquisitionComp::DeviceInfoList::IsOptionEnabled(int index) const
{
	Q_UNUSED(index);

	return true;
}


} // namespace iocv


