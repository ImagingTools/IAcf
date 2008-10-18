#ifndef icntl_IMultiLineController_included
#define icntl_IMultiLineController_included


// ACF includes
#include "istd/IPolymorphic.h"

#include "icntl/icntl.h"


namespace icntl
{


class ILineController;


/**
	Interfce to controlling set of production lines.
*/
class IMultiLineController: virtual public istd::IPolymorphic
{
public:
	/**
		Get number of lines in this multi line system.
	*/
	virtual int GetLinesCount() const = 0;

	/**
		Get access to single line using its index.
	*/
	virtual ILineController& GetLineController(int lineIndex) const = 0;
};


} // namespace icntl


#endif // !icntl_IMultiLineController_included


