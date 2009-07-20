#ifndef iqtcam_CMemoryAcquisitionComp_included
#define iqtcam_CMemoryAcquisitionComp_included


// Qt includes
#include <QDir>
#include <QStringList>

// ACF includes
#include "iser/IFileLoader.h"
#include "icomp/CComponentBase.h"
#include "iprm/IFileNameParam.h"
#include "iqt/CBitmap.h"

#include "iproc/TSyncProcessorWrap.h"

#include "icam/IBitmapAcquisition.h"

#include "iqtcam/iqtcam.h"


namespace iqtcam
{


/**
	Bitmap loader component implementing interfaces \c iser::IFileLoader and \c icam::IBitmapAcquisition.
*/
class CMemoryAcquisitionComp:
			public icomp::CComponentBase,
			public iqt::CBitmap,
			virtual public iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef iqt::CBitmap BaseClass2;

	I_BEGIN_COMPONENT(CMemoryAcquisitionComp);
		I_REGISTER_INTERFACE(icam::IBitmapAcquisition);
		I_REGISTER_INTERFACE(iimg::IBitmap);
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr);

	// reimplemented (icam::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;
};


} // namespace iqtcam


#endif // !iqtcam_CMemoryAcquisitionComp_included


