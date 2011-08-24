#ifndef isig_ITriggerParams_included
#define isig_ITriggerParams_included


#include "iser/ISerializable.h"

#include "isig/isig.h"


namespace isig
{


class ITriggerConstraints;


/**
	Interface for AVT camera parameters.
*/
class ITriggerParams: virtual public iser::ISerializable
{
public:
	/**
		Describe working mode of trigger signal.
	*/
	enum TriggerMode
	{
		TM_DEFAULT,
		TM_RISING_EDGE,
		TM_FALLING_EDGE,
		TM_POSITIVE_LEVEL,
		TM_NEGATIVE_LEVEL
	};

	/**
		Get constrints describing supported parameter states.
	*/
	virtual const ITriggerConstraints* GetConstraints() const = 0;

	/**
		Check if trigger is enabled.
	*/
	virtual bool IsTriggerEnabled() const = 0;
	/**
		Set trigger enabling state.
	*/
	virtual void SetTriggerEnabled(bool isEnabled) = 0;

	/**
		Get trigger mode.
		Normal trigger modes are defined in \c isig::ITriggerParams::TriggerMode.
	*/
	virtual int GetTriggerMode() const = 0;

	/**
		Set trigger mode.
		Normal trigger modes are defined in \c isig::ITriggerParams::TriggerMode.
	*/
	virtual void SetTriggerMode(int triggerMode) = 0;
};


} // namespace isig


#endif // !isig_ITriggerParams_included


