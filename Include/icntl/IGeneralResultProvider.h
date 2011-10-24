#ifndef icntl_IGeneralResultProvider_included
#define icntl_IGeneralResultProvider_included


// ACF includes
#include "istd/IChangeable.h"


namespace icntl
{


/**
	Provide general inspection result.
*/
class IGeneralResultProvider: virtual public istd::IChangeable
{
public:
	enum GeneralResult
	{
		GR_OK,
		GR_WARNING,
		GR_FAILED,
		GR_CRITICAL
	};

	virtual GeneralResult GetGeneralResult() const = 0;
};


} // namespace icntl


#endif // !icntl_IGeneralResultProvider_included


