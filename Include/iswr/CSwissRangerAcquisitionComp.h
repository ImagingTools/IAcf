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


namespace iswr
{


/**
	SwissRanger based camera component implementing interfaces 
	\c icam::IBitmapAcquisition and \c icam::IExposureConstraints for exposure control support.
*/
class CSwissRangerAcquisitionComp:
			public ibase::TLoggerCompWrap<icomp::CComponentBase>,
			virtual public iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>,
			virtual public icam::IExposureConstraints

{
public:
	typedef ibase::TLoggerCompWrap<icomp::CComponentBase> BaseClass;

	I_BEGIN_COMPONENT(CSwissRangerAcquisitionComp);
		I_REGISTER_INTERFACE(icam::IBitmapAcquisition);
		I_REGISTER_INTERFACE(icam::IExposureConstraints);
		I_ASSIGN(m_defaultExposureParamsCompPtr, "DefaultExposureParams", "Default exposure parameters will be used if no parameters are found", false, "DefaultExposureParams");
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

	// reimplemented (icam::IExposureConstraints)
	virtual istd::CRange GetShutterTimeRange() const;
	virtual istd::CRange GetDelayTimeRange() const;
	virtual istd::CRange GetEenDelayRange() const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected:
	/**
		Check whether parameter was set correctly.
	*/
	bool CheckParameter(UINT16 parameterId, UINT32 setValue);
	
private:
	CMesaDevice* m_cameraPtr;

	ImgEntry* m_imgEntryArray;
	int m_imagesCount;
	istd::TDelPtr<float, true> m_xBuffer;
	istd::TDelPtr<float, true> m_yBuffer;
	istd::TDelPtr<float, true> m_zBuffer;

	I_REF(icam::IExposureParams, m_defaultExposureParamsCompPtr);
	I_ATTR(istd::CString, m_exposureParamsIdAttrPtr);
	I_ATTR(istd::CString, m_swissRangerParamsIdAttrPtr);
	I_ATTR(double, m_timeoutAttrPtr);
};


} // namespace iswr


#endif // !iqt_CSwissRangerAcquisitionComp_included


