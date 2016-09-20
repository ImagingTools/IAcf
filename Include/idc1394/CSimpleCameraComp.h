#ifndef idc1394_CSimpleCameraComp_included
#define idc1394_CSimpleCameraComp_included


// DC1394 includes
#include <dc1394/dc1394.h>

// ACF includes
#include <i2d/CRectangle.h>
#include <ilog/TLoggerCompWrap.h>
#include <icomp/CComponentBase.h>

// ACF-Solutions includes
#include <iproc/TSyncProcessorWrap.h>
#include <icam/IBitmapAcquisition.h>
#include <icam/IExposureConstraints.h>
#include <icam/IExposureParams.h>
#include <imeas/ILinearAdjustParams.h>
#include <imeas/ILinearAdjustConstraints.h>


namespace idc1394
{


/**
	Simple fire wire camera implementation based on library libDC1394.
*/
class CSimpleCameraComp:
			public ilog::CLoggerComponentBase,
			virtual public iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSimpleCameraComp);
		I_REGISTER_INTERFACE(icam::IBitmapAcquisition);
		I_REGISTER_SUBELEMENT(AdjustConstraints);
		I_REGISTER_SUBELEMENT_INTERFACE_T(AdjustConstraints, imeas::ILinearAdjustConstraints, ExtractAdjustContraints);
		I_REGISTER_SUBELEMENT_INTERFACE_T(AdjustConstraints, istd::IChangeable, ExtractAdjustContraints);
		I_REGISTER_SUBELEMENT(ExposureConstraints);
		I_REGISTER_SUBELEMENT_INTERFACE_T(ExposureConstraints, icam::IExposureConstraints, ExtractExposureContraints);
		I_REGISTER_SUBELEMENT_INTERFACE_T(ExposureConstraints, istd::IChangeable, ExtractExposureContraints);
		I_ASSIGN(m_adjustParamsIdAttrPtr, "AdjustParamsId", "ID used to get brightness and contrast adjust parameters from the parameter set", false, "AdjustParams");
		I_ASSIGN(m_defaultAdjustParamsCompPtr, "DefaultAdjustParams", "Default contrast and brightness adjust parameters that will be used", false, "DefaultAdjustParams");
		I_ASSIGN(m_exposureParamsIdAttrPtr, "ExposureParamsId", "ID used to exposure parameters from the parameter set", false, "ExposureParams");
		I_ASSIGN(m_defaultExposureParamsCompPtr, "DefaultExposureParams", "Default exposure parameters that will be used", false, "DefaultExposureParams");
		I_ASSIGN(m_only8BitDepthAttrPtr, "Only8Bit", "Only modes with 8 bits will be accepted", true, true);
		I_ASSIGN(m_bufferSizeAttrPtr, "BufferSize", "Number of frames in camera buffer", true, 1);
		I_ASSIGN(m_waitTimeoutAttrPtr, "WaitTimeout", "Wait timeout for capture, if not set only frames currently acquired will be taken", false, 1);
	I_END_COMPONENT;

	enum MessageId
	{
		MI_CANNOT_FIND_CAMERA = 0xdf271,
		MI_CAMERA
	};

	CSimpleCameraComp();

	// reimplemented (iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (icam::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented (iproc::IProcessor)
	virtual void InitProcessor(const iprm::IParamsSet* paramsPtr);

protected:
	/**
		Set parameters from parameter set to camera.
	*/
	void SetParametersToCamera(const iprm::IParamsSet* paramsPtr);

	bool GetAbsoluteFeatureRange(dc1394camera_t& camera, dc1394feature_t feature, istd::CRange& range) const;
	bool SetAbsoluteFeatureValue(dc1394camera_t& camera, dc1394feature_t feature, double value);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	class AdjustConstraints: public imeas::ILinearAdjustConstraints
	{
	public:
		// reimplemented (imeas::ILinearAdjustConstraints)
		virtual istd::CRange GetScaleFactorRange() const;
		virtual istd::CRange GetOffsetFactorRange() const;

		istd::CRange scaleRange;
		istd::CRange offsetRange;
	};

	class ExposureConstraints: public icam::IExposureConstraints
	{
	public:
		// reimplemented (icam::IExposureConstraints)
		virtual istd::CRange GetShutterTimeRange() const;
		virtual istd::CRange GetDelayTimeRange() const;
		virtual istd::CRange GetEenDelayRange() const;

		istd::CRange shutterTimeRange;
	};

	I_ATTR(QByteArray, m_adjustParamsIdAttrPtr);
	I_REF(imeas::ILinearAdjustParams, m_defaultAdjustParamsCompPtr);
	I_ATTR(QByteArray, m_exposureParamsIdAttrPtr);
	I_REF(icam::IExposureParams, m_defaultExposureParamsCompPtr);
	I_ATTR(bool, m_only8BitDepthAttrPtr);
	I_ATTR(int, m_bufferSizeAttrPtr);
	I_ATTR(double, m_waitTimeoutAttrPtr);
	dc1394_t* m_libraryPtr;
	dc1394camera_t* m_cameraPtr;

	AdjustConstraints m_adjustConstraints;
	ExposureConstraints m_exposureConstraints;

	// static template methods for subelement access
	template <class InterfaceType>
	static InterfaceType* ExtractAdjustContraints(CSimpleCameraComp& component)
	{
		return &component.m_adjustConstraints;
	}

	// static template methods for subelement access
	template <class InterfaceType>
	static InterfaceType* ExtractExposureContraints(CSimpleCameraComp& component)
	{
		return &component.m_exposureConstraints;
	}
};


} // namespace idc1394


#endif // !idc1394_CSimpleCameraComp_included


