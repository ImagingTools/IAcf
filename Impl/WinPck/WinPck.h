#ifndef WinPck_included
#define WinPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

// IACF includes
#include "iwin/CTimeStampCacheComp.h"


/**
	Windows dependent package.
*/
namespace WinPck
{


typedef icomp::TModelCompWrap<iwin::CTimeStampCacheComp> TimeStampCache;


} // namespace WinPck


#endif // !WinPck_included


