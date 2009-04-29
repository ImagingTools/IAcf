#ifndef idev_IDeviceInfo_included
#define idev_IDeviceInfo_included


#include "istd/CString.h"

#include "iser/ISerializable.h"


namespace idev
{


/**
	An interface for device description.
*/
class IDeviceInfo: virtual public iser::ISerializable
{
public:
	/**
		Get device id.
	*/
	virtual istd::CString GetDeviceId() const = 0;
	
	/**
		Set device id.
	*/
	virtual void SetDeviceId(const istd::CString& deviceId) = 0;

	/**
		Get device driver version.
	*/
	virtual I_DWORD GetDeviceDriverVersion() const = 0;

	/**
		Set device driver version.
	*/
	virtual void SetDeviceDriverVersion(I_DWORD deviceVersion) = 0;

	/**
		Gets device API version.
	*/
	virtual I_DWORD GetDeviceLibraryVersion() const = 0;

	/**
		Set device driver version.
	*/
	virtual void SetDeviceLibraryVersion(I_DWORD libararyVersion) = 0;
};



} // namespace idev



#endif //!INCLUDED_idev_IDeviceInfo_h



