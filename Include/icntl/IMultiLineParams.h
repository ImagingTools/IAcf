#ifndef icntl_IMultiLineParams_included
#define icntl_IMultiLineParams_included


// ACF includes
#include "istd/INamed.h"
#include "iser/ISerializable.h"

#include "icntl/icntl.h"


namespace icntl
{


class ILineParams;


class IMultiLineParams:
			virtual public iser::ISerializable,
			virtual public istd::INamed,
{
public:
	/**
		Get number of lines in this multi line system.
	*/
	virtual int GetLinesCount() const = 0;

	/**
		Get access to single line using its index.
	*/
	virtual ILineParams& GetLineParams(int lineIndex) const = 0;
};


} // namespace icntl


#endif // !icntl_IMultiLineParams_included


