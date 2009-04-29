#ifndef idev_IDeviceDescription_included
#define idev_IDeviceDescription_included


#include "istd/CString.h"


namespace idev
{


/**
	An interface for device description.
*/
class IDeviceDescription: virtual public iser::ISerializable
{
public:
	/**
		Get device description.
	*/
	virtual istd::CString GetDeviceDescription() const = 0;

	/**
		Set device description.
	*/
	virtual void SetDeviceDescription(const istd::CString& deviceDescription) = 0;

	/**
		Get device name.
	*/
	virtual istd::CString GetDeviceName() const = 0;

	/**
		Set device name. \c deviceName is a free user defined string.
	*/
	virtual void SetDeviceName(const istd::CString& deviceName) = 0;
};


} // namespace idev


#endif //!INCLUDED_idev_IDeviceDescription_h


