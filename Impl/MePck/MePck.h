#ifndef MePck_included
#define MePck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "isig/CSamplingParamsComp.h"

#include "CMeilhausSimpleComp.h"
#include "CChannelAddressParamComp.h"



/**
	Package for components depending on I/O component of Meilhaus GmbH company.
*/
namespace MePck
{


typedef icomp::TModelCompWrap<isig::CSamplingParamsComp> SamplingParams;

typedef imebase::CMeilhausSimpleComp MeilhausSimpleAcquisition;
typedef icomp::TModelCompWrap<imebase::CChannelAddressParamComp> ChannelAddressParam;


} // namespace MePck


#endif // !MePck_included


