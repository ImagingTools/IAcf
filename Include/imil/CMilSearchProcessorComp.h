#ifndef imil_CMilSearchProcessorComp_included
#define imil_CMilSearchProcessorComp_included


#include "ibase/TLoggerCompWrap.h"

#include "iipr/IFeaturesConsumer.h"

#include "iproc/TSyncProcessorCompBase.h"

#include "iimg/IBitmap.h"

#include "imil/CMilSearchParams.h"
#include "imil/CMilSearchModel.h"
#include "imil/CMilEngine.h"


namespace imil
{


class CMilSearchProcessorComp: public ibase::TLoggerCompWrap<iproc::CSyncProcessorCompBase>
{
public:
	typedef ibase::TLoggerCompWrap<iproc::CSyncProcessorCompBase> BaseClass;

	I_BEGIN_COMPONENT(CMilSearchProcessorComp)
		I_ASSIGN(m_paramsIdAttrPtr, "ParamsId", "ID of processor parameter", true, "ParamsId");
	I_END_COMPONENT

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr);

protected:
	virtual bool DoSearch(
				const CMilSearchParams& params,
				const iimg::IBitmap& bitmap,
				iipr::IFeaturesConsumer& result);

private:
	imil::CMilEngine m_engine;

	I_ATTR(istd::CString, m_paramsIdAttrPtr);
};


} // namespace iipr


#endif // !imil_CMilSearchProcessorComp_included


