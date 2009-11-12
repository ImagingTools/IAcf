#ifndef IacfQtPck_included
#define IacfQtPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqtsig/CSamplingParamsGuiComp.h"
#include "iqtsig/CSampleAcquisitionGuiComp.h"
#include "iqtsig/CScriptSampleAcquisitionComp.h"


/**
	Standard Qt package.
*/
namespace IacfQtPck
{


typedef iqtsig::CSamplingParamsGuiComp SamplingParamsGui;
typedef iqtsig::CSampleAcquisitionGuiComp SampleAcquisitionGui;
typedef iqtsig::CScriptSampleAcquisitionComp ScriptSampleAcquisition;


} // namespace IacfQtPck


#endif // !IacfQtPck_included


