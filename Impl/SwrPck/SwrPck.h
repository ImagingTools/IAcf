#ifndef SwrPck_included
#define SwrPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iswr/CSwissRangerAcquisitionComp.h"
#include "iswr/CSwissRangerParams.h"
#include "iswr/CSwissRangerParamsComp.h"
#include "iswr/CSwissRangerAcquisitionData.h"


/**
	SwissRanger-specific package.
*/
namespace SwrPck
{


typedef iswr::CSwissRangerAcquisitionComp SwissRangerAcquisition;
typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						iswr::CSwissRangerParams, 
						iswr::ISwissRangerParams, 
						iser::ISerializable> > SwissRangerParams;

typedef icomp::TModelCompWrap<iswr::CSwissRangerParamsComp> AttributedSwissRangerParams;

typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						iswr::CSwissRangerAcquisitionData, 
						iswr::ISwissRangerAcquisitionData, 
						iser::ISerializable> > SwissRangerAcquisitionData;


} // namespace SwrPck


#endif // !SwrPck_included


