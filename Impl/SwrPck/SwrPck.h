#ifndef SwrPck_included
#define SwrPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iswr/CSwissRangerAcquisitionComp.h"
#include "iswr/CSwissRangerParams.h"



/**
	SwissRanger-specific package.
*/
namespace SwrPck
{


typedef iswr::CSwissRangerAcquisitionComp SwissRangerAcquisition;
typedef icomp::TModelCompWrap<
icomp::TMakeComponentWrap<iswr::CSwissRangerParams, iswr::ISwissRangerParams, iser::ISerializable> > SwissRangerParams;


} // namespace SwrPck


#endif // !SwrPck_included


