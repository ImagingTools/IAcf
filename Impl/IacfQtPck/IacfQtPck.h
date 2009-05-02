#ifndef QtPck_included
#define QtPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqtproc/CProgressManagerGuiComp.h"
#include "iqtproc/CAcquisitonProcessorGuiComp.h"

#include "iqtsig/CSamplingParamsGuiComp.h"
#include "iqtsig/CSampleAcquisitionGuiComp.h"
#include "iqtsig/CScriptSampleAcquisitionComp.h"


/**
	Standard Qt package.
*/
namespace IacfQtPck
{


typedef iqtproc::CProgressManagerGuiComp ProgressManagerGui;
typedef iqtproc::CAcquisitonProcessorGuiComp AcquisitionProcessorGui;

typedef iqtsig::CSamplingParamsGuiComp SamplingParamsGui;
typedef iqtsig::CSampleAcquisitionGuiComp SampleAcquisitionGui;
typedef iqtsig::CScriptSampleAcquisitionComp ScriptSampleAcquisition;


} // namespace IacfQtPck


#endif // !QtPck_included


