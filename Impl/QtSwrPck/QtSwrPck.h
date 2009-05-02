#ifndef QtSwrPck_included
#define QtSwrPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"


#include "iqtswr/CSwissRangerParamsGuiComp.h"
#include "iqtswr/CSwissRangerImageViewComp.h"


/**
	Qt based SwissRanger-specific package.
*/
namespace QtSwrPck
{

typedef iqtswr::CSwissRangerParamsGuiComp SwissRangerParamsGui;
typedef iqtswr::CSwissRangerImageViewComp SwissRangerImageView;

} // namespace QtSwrPck


#endif // !QtSwrPck_included


