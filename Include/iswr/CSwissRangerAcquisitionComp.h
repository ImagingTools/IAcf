#ifndef iqt_CSwissRangerAcquisitionComp_included
#define iqt_CSwissRangerAcquisitionComp_included


// SwissRanger includes
#include <windows.h>
#include <libusbSR.h>


// ACF includes
#include "istd/TDelPtr.h"

#include "i2d/CRectangle.h"

#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"

// IACF includes
#include "iproc/TSyncProcessorWrap.h"

#include "icam/IBitmapAcquisition.h"
#include "icam/IExposureConstraints.h"
#include "icam/IExposureParams.h"

#include "idev/IDeviceInfo.h"

#include "iswr/ISwissRangerConstrains.h"
#include "iswr/ISwissRangerImage.h"


namespace iswr
{


/**
	SwissRanger based camera component implementing interfaces 
	\c icam::IBitmapAcquisition and \c icam::IExposureConstraints for exposure control support.
*/
class CSwissRangerAcquisitionComp:
			public ibase::TLoggerCompWrap<icomp::CComponentBase>,
			virtual public iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>,
			virtual public icam::IExposureConstraints,
			virtual public iswr::ISwissRangerConstrains

{
public:
	typedef ibase::TLoggerCompWrap<icomp::CComponentBase> BaseClass;

	I_BEGIN_COMPONENT(CSwissRangerAcquisitionComp);
		I_REGISTER_INTERFACE(icam::IBitmapAcquisition);
		I_REGISTER_INTERFACE(icam::IExposureConstraints);
		I_REGISTER_INTERFACE(iswr::ISwissRangerConstrains);
		I_ASSIGN(m_defaultExposureParamsCompPtr, "DefaultExposureParams", "Default exposure parameters will be used if no parameters are found", false, "DefaultExposureParams");
		I_ASSIGN(m_deviceInfoCompPtr, "DeviceInfo", "Info about the SwissRanger sensor", false, "DeviceInfo");
		I_ASSIGN(m_exposureParamsIdAttrPtr, "ExposureParamsId", "Id used to get exposure parameters from the parameter set", false, "ExposureParamsId");
		I_ASSIGN(m_swissRangerParamsIdAttrPtr, "SwissRangerParamsId", "Id used to get swiss ranger parameters from the parameter set", false, "SwissRangerParamsId");
		I_ASSIGN(m_timeoutAttrPtr, "Timeout", "Acquisition timeout", true, 5.0);
	I_END_COMPONENT;

	enum MessageId
	{
		MI_CANNOT_INIT = 0x28224fa,
		MI_CANNOT_CONNECT,
		MI_CANNOT_OPEN,
		MI_CANNOT_START,
	};

	CSwissRangerAcquisitionComp();

	bool IsCameraValid() const;

	// reimplemented (iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr);

	// reimplemented (icam::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented (iswr::ISwissRangerConstrains)
	virtual const SupportedFrequencies& GetSupportedFrequences() const;

	// reimplemented (icam::IExposureConstraints)
	virtual istd::CRange GetShutterTimeRange() const;
	virtual istd::CRange GetDelayTimeRange() const;
	virtual istd::CRange GetEenDelayRange() const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	bool CreateSwissImage(iswr::ISwissRangerImage& swissImage) const;
	bool CreateOutputBitmap(
				iimg::IBitmap& bitmap,
				double maxDistance, 
				const istd::CRange& clippingDistanceRange) const;

private:
	CMesaDevice* m_cameraPtr;

	ImgEntry* m_imgEntryArray;
	int m_imagesCount;
	istd::TDelPtr<float, true> m_xBuffer;
	istd::TDelPtr<float, true> m_yBuffer;
	istd::TDelPtr<float, true> m_zBuffer;

	SupportedFrequencies m_supportedFrequencies;

	I_REF(icam::IExposureParams, m_defaultExposureParamsCompPtr);
	I_REF(idev::IDeviceInfo, m_deviceInfoCompPtr);
	I_ATTR(istd::CString, m_exposureParamsIdAttrPtr);
	I_ATTR(istd::CString, m_swissRangerParamsIdAttrPtr);
	I_ATTR(double, m_timeoutAttrPtr);
};


} // namespace iswr


#endif // !iqt_CSwissRangerAcquisitionComp_included


