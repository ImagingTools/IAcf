#ifndef QtSwrPck_included
#define QtSwrPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqtswr/CSwissRangerParamsGuiComp.h"
#include "iqtswr/CSwissRangerAcquisitionDataViewComp.h"


/**
	Qt based SwissRanger-specific package.
*/
namespace QtSwrPck
{

typedef iqtswr::CSwissRangerParamsGuiComp SwissRangerParamsGui;
typedef iqtswr::CSwissRangerAcquisitionDataViewComp SwissRangerAcquisitionDataView;


} // namespace QtSwrPck


#endif // !QtSwrPck_included


