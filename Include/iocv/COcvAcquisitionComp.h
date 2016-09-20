#ifndef iocv_COcvAcquisitionComp_included
#define iocv_COcvAcquisitionComp_included


// OpenCV includes
#include <opencv/cv.h>
#include <opencv/highgui.h>
#undef min
#undef max

// ACF includes
#include <i2d/CRectangle.h>
#include <ilog/TLoggerCompWrap.h>
#include <icomp/CComponentBase.h>
#include <iproc/TSyncProcessorWrap.h>

// ACF-Solutions includes
#include <icam/IBitmapAcquisition.h>
#include <icam/IExposureConstraints.h>
#include <icam/IExposureParams.h>
#include <isig/ITriggerParams.h>
#include <isig/ITriggerConstraints.h>


namespace iocv
{


/**
	Open CV based camera component implementation.
*/
class COcvAcquisitionComp:
			public ilog::CLoggerComponentBase,
			virtual public isig::ITriggerConstraints,
			virtual public iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(COcvAcquisitionComp);
		I_REGISTER_INTERFACE(icam::IBitmapAcquisition);
		I_REGISTER_INTERFACE(isig::ITriggerConstraints);
	I_END_COMPONENT;

	enum MessageId
	{
	};

	COcvAcquisitionComp();

	bool IsCameraValid() const;

	// reimplemented (iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (icam::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented (isig::ITriggerConstraints)
	virtual bool IsTriggerModeSupported(int triggerMode) const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();
	
private:
	I_REF(icam::IExposureParams, m_defaultExposureParamsCompPtr);
	I_REF(isig::ITriggerParams, m_triggerParamsCompPtr);
	I_REF(i2d::CRectangle, m_imageRegionParamsCompPtr);

	I_ATTR(bool, m_singleShootAttrPtr);
	I_ATTR(double, m_timeoutAttrPtr);

	CvCapture* m_cameraPtr;
};


} // namespace iocv


#endif // !iocv_COcvAcquisitionComp_included


