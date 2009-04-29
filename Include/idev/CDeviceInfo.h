#ifndef idev_CDeviceInfo_included
#define idev_CDeviceInfo_included


#include "idev/IDeviceInfo.h"


namespace idev
{


class CDeviceInfo: virtual public idev::IDeviceInfo
{
public:
	CDeviceInfo();

	// reimplemented (idev::IDeviceInfo)
	virtual istd::CString GetDeviceId() const;
	virtual void SetDeviceId(const istd::CString& deviceId);
	virtual I_DWORD GetDeviceDriverVersion() const;
	virtual void SetDeviceDriverVersion(I_DWORD deviceVersion);
	virtual I_DWORD GetDeviceLibraryVersion() const;
	virtual void SetDeviceLibraryVersion(I_DWORD libararyVersion);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	istd::CString m_deviceId;
	istd::CString m_deviceDriverVersion;
	istd::CString m_deviceLibraryVersion;
};


} // namespace idev


#endif //!idev_CDeviceInfo_included


