#ifndef IacfQtPck_included
#define IacfQtPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqtsig/CSamplingParamsGuiComp.h"
#include "iqtsig/CSampleAcquisitionGuiComp.h"
#include "iqtsig/CScriptDataProcessorComp.h"
#include "iqtinsp/CInspectionTaskGuiComp.h"
#include "iqtinsp/CGeneralSupplierGuiComp.h"


/**
	Standard Qt package.
*/
namespace IacfQtPck
{


typedef iqtsig::CSamplingParamsGuiComp SamplingParamsGui;
typedef iqtsig::CSampleAcquisitionGuiComp SampleAcquisitionGui;
typedef iqtsig::CScriptDataProcessorComp ScriptDataProcessor;
typedef iqtinsp::CInspectionTaskGuiComp InspectionTaskGui;
typedef iqtinsp::CGeneralSupplierGuiComp GeneralSupplierGui;


} // namespace IacfQtPck


#endif // !IacfQtPck_included


