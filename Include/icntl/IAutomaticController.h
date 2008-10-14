#ifndef icntl_IAutomaticController_included
#define icntl_IAutomaticController_included


// ACF includes
#include "istd/IChangeable.h"


namespace icntl
{


/**
	Allows to control working mode of control system.
*/
class IAutomaticController: virtual public istd::IChangeable
{
public:
	enum WorkMode
	{
		WM_NONE,
		WM_MANUAL,
		WM_AUTOMATIC
	};

	/**
		Check if automatic is on.
	*/
	virtual int GetWorkMode() const = 0;
	/**
		Set automatic on or off.
		\return	true if automatic mode was switched to specified.
	*/
	virtual bool SetWorkMode(int mode) = 0;
	/**
		Check if some work mode is supported.
	*/
	virtual bool IsModeSupported(int mode) const = 0;
};


} // namespace icntl


#endif // !icntl_IAutomaticController_included


