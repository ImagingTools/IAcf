#ifndef IacfQtPck_included
#define IacfQtPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqtsig/CSamplingParamsGuiComp.h"
#include "iqtsig/CSampleAcquisitionGuiComp.h"
#include "iqtsig/CScriptDataProcessorComp.h"


/**
	Standard Qt package.
*/
namespace IacfQtPck
{


typedef iqtsig::CSamplingParamsGuiComp SamplingParamsGui;
typedef iqtsig::CSampleAcquisitionGuiComp SampleAcquisitionGui;
typedef iqtsig::CScriptDataProcessorComp ScriptDataProcessor;


} // namespace IacfQtPck


#endif // !IacfQtPck_included


