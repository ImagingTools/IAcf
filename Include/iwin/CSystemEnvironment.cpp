#include "iwin/CSystemEnvironment.h"


// WINFOWS includes
#include <windows.h>


namespace iwin
{


// reimplemented (isys::ISystemEnvironment)

QString CSystemEnvironment::GetUser() const
{
	return GetSystemVariableValue("USER");
}


QString CSystemEnvironment::GetTempDirPath() const
{
	return GetSystemVariableValue("TEMP");
}


QString CSystemEnvironment::GetUniqueIdentifier() const
{
	// TODO: move to basics
	GUID guid;
	::UuidCreate(&guid);
	RPC_CSTR guidBstr = NULL;
	::UuidToString(&guid, &guidBstr);

	QString uniqueIdentifier = QString((char*)guidBstr);
	
	::RpcStringFree(&guidBstr);

	return uniqueIdentifier;
}


} // namespace isys


