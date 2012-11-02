#ifndef idc1394_CSimpleCameraComp_included
#define idc1394_CSimpleCameraComp_included


// DC1394 includes
#include <dc1394/dc1394.h>

// ACF includes
#include "i2d/CRectangle.h"
#include "ibase/TLoggerCompWrap.h"
#include "icomp/CComponentBase.h"
#include "iproc/TSyncProcessorWrap.h"

// ACF-Solutions includes
#include "icam/IBitmapAcquisition.h"


namespace idc1394
{


/**
	Simple fire wire camera implementation based on library libDC1394.
*/
class CSimpleCameraComp:
			public ibase::CLoggerComponentBase,
			virtual public iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSimpleCameraComp);
		I_REGISTER_INTERFACE(icam::IBitmapAcquisition);
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

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	dc1394_t * m_libraryPtr;
	dc1394camera_t *m_cameraPtr;
};


} // namespace idc1394


#endif // !idc1394_CSimpleCameraComp_included


