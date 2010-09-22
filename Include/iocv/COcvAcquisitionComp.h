#ifndef iocv_COcvAcquisitionComp_included
#define iocv_COcvAcquisitionComp_included


// OpenCV includes
#include "cxtypes.h"
#include "cv.h"
#include "highgui.h"


// ACF includes
#include "ibase/TLoggerCompWrap.h"

#include "i2d/CRectangle.h"

#include "iproc/TSyncProcessorWrap.h"

#include "iproc/IBitmapAcquisition.h"
#include "icam/IExposureConstraints.h"
#include "icam/ITriggerConstraints.h"
#include "icam/IExposureParams.h"
#include "icam/ITriggerParams.h"

#include "icomp/CComponentBase.h"


namespace iocv
{


/**
	Open CV based camera component implementation.
*/
class COcvAcquisitionComp:
			public ibase::CLoggerComponentBase,
			virtual public icam::ITriggerConstraints,
			virtual public iproc::TSyncProcessorWrap<iproc::IBitmapAcquisition>
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(COcvAcquisitionComp);
		I_REGISTER_INTERFACE(iproc::IBitmapAcquisition);
		I_REGISTER_INTERFACE(icam::ITriggerConstraints);
	I_END_COMPONENT;

	enum MessageId
	{
	};

	COcvAcquisitionComp();

	bool IsCameraValid() const;

	// reimplemented (iproc::TSyncProcessorWrap<iproc::IBitmapAcquisition>)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (iproc::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented (icam::ITriggerConstraints)
	virtual bool IsTriggerSupported() const;
	virtual bool IsTriggerModeSupported(int triggerMode) const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();
	
private:
	I_REF(icam::IExposureParams, m_defaultExposureParamsCompPtr);
	I_REF(icam::ITriggerParams, m_triggerParamsCompPtr);
	I_REF(i2d::CRectangle, m_imageRegionParamsCompPtr);
	I_ATTR(istd::CString, m_exposureParamsIdAttrPtr);

	I_ATTR(bool, m_singleShootAttrPtr);
	I_ATTR(double, m_timeoutAttrPtr);

	CvCapture* m_cameraPtr;
};


} // namespace iocv


#endif // !iocv_COcvAcquisitionComp_included


