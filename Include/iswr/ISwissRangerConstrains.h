#ifndef iswr_ISwissRangerConstrains_included
#define iswr_ISwissRangerConstrains_included


#include "iswr/iswr.h"


// ACF includes
#include "istd/IPolymorphic.h"


namespace iswr
{


/**
	Parameters od SwissRanger sensor
*/
class ISwissRangerConstrains: virtual public istd::IPolymorphic
{
public:
	typedef std::set<int> SupportedFrequencies;

	virtual const SupportedFrequencies& GetSupportedFrequences() const = 0;
};


} // namespace iswr


#endif // !iswr_ISwissRangerParams_included


