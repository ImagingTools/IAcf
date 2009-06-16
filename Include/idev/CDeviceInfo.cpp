#include "idev/CDeviceInfo.h"


namespace idev
{


CDeviceInfo::CDeviceInfo()
{
}


// reimplemented (idev::IDeviceInfo)

istd::CString CDeviceInfo::GetDeviceId() const
{
	return m_deviceId;
}


void CDeviceInfo::SetDeviceId(const istd::CString& /*deviceId*/)
{
}


I_DWORD CDeviceInfo::GetDeviceDriverVersion() const
{
	return m_deviceDriverVersion;
}


void CDeviceInfo::SetDeviceDriverVersion(I_DWORD /*deviceVersion*/)
{
}


I_DWORD CDeviceInfo::GetDeviceLibraryVersion() const
{
	return m_deviceLibraryVersion;
}


void CDeviceInfo::SetDeviceLibraryVersion(I_DWORD /*libararyVersion*/)
{
}


// reimplemented (iser::ISerializable)

bool CDeviceInfo::Serialize(iser::IArchive& /*archive*/)
{
	return true;
}


} // namespace idev


