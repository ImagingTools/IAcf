#ifndef MePck_included
#define MePck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

#include <MePck/CMeilhausSimpleComp.h>
#include <MePck/CChannelAddressParamComp.h>



/**
	Package for components depending on I/O component of Meilhaus GmbH company.
*/
namespace MePck
{


typedef imebase::CMeilhausSimpleComp MeilhausSimpleAcquisition;
typedef icomp::TModelCompWrap<imebase::CChannelAddressParamComp> ChannelAddressParam;


} // namespace MePck


#endif // !MePck_included


