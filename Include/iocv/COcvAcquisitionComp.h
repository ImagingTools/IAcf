#ifndef iocv_COcvAcquisitionComp_included
#define iocv_COcvAcquisitionComp_included


// OpenCV includes
#include <opencv2/opencv.hpp>

// ACF includes
#include <istd/TPointerVector.h>
#include <imod/TModelWrap.h>
#include <iprm/IOptionsList.h>
#include <ilog/TLoggerCompWrap.h>
#include <icomp/CComponentBase.h>
#include <iproc/TSyncProcessorWrap.h>

// ACF-Solutions includes
#include <icam/IBitmapAcquisition.h>


namespace iocv
{


/**
	Open CV based camera component implementation.
*/
class COcvAcquisitionComp:
			public ilog::CLoggerComponentBase,
			public iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(COcvAcquisitionComp);
		I_REGISTER_INTERFACE(icam::IBitmapAcquisition);
		I_REGISTER_INTERFACE(iproc::IProcessor);
		I_REGISTER_SUBELEMENT(DeviceList);
		I_REGISTER_SUBELEMENT_INTERFACE(DeviceList, iprm::IOptionsList, ExtractDeviceList);
		I_REGISTER_SUBELEMENT_INTERFACE(DeviceList, istd::IChangeable, ExtractDeviceList);
		I_REGISTER_SUBELEMENT_INTERFACE(DeviceList, imod::IModel, ExtractDeviceList);
		I_ASSIGN(m_cameraIndexAttrPtr, "CameraSelectionId", "ID of the camera selector in the parameter set", true, "CameraIndex");
	I_END_COMPONENT;

	COcvAcquisitionComp();

	// reimplemented (iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>)
	virtual iproc::IProcessor::TaskState DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL) override;

	// reimplemented (icam::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const override;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	class CameraDevice: public cv::VideoCapture
	{
	public:
		typedef cv::VideoCapture BaseClass;

		CameraDevice(int cameraDriverId, const QString& deviceName);
		CameraDevice(const QString& streamUrl, const QString& deviceName);

		QString GetDeviceName() const;

	private:
		QString m_deviceName;
	};


	typedef istd::TPointerVector<CameraDevice> CameraDeviceList;

	/**
		Enumerate all connnected camera devices.
	*/
	void EnumerateCameraDevices();

	/**
		Get currently selected camera according to given parameters.
	*/
	CameraDevice* GetSelectedCameraDevice(const iprm::IParamsSet* paramsPtr) const;
	
private:
	class DeviceInfoList: virtual public iprm::IOptionsList
	{
	public:
		DeviceInfoList();

		void SetParent(COcvAcquisitionComp* parentPtr);

		// reimplemented (iprm::IOptionsList)
		virtual int GetOptionsFlags() const;
		virtual int GetOptionsCount() const;
		virtual QString GetOptionName(int index) const;
		virtual QString GetOptionDescription(int index) const;
		virtual QByteArray GetOptionId(int index) const;
		virtual bool IsOptionEnabled(int index) const;
		
	private:
		COcvAcquisitionComp* m_parentPtr;
	};

	template <class InterfaceType>
	static InterfaceType* ExtractDeviceList(COcvAcquisitionComp& component)
	{
		return &component.m_deviceInfoList;
	}

	/**
		Current available devices.
	*/
	CameraDeviceList m_deviceList;

	typedef QMap<int /*OpenCV ID*/, QString /*Driver name*/> CameraDriversMap;

	/**
		All supported camera backends.
	*/
	CameraDriversMap m_supportedCameraDriversMap;

	/**
		ID of the camera selector in the parameter set.
	*/
	I_ATTR(QByteArray, m_cameraIndexAttrPtr);

	imod::TModelWrap<DeviceInfoList> m_deviceInfoList;
};


} // namespace iocv


#endif // !iocv_COcvAcquisitionComp_included


