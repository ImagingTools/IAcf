#include "iwin/CDllFunctionsProvider.h"


namespace iwin
{


CDllFunctionsProvider::CDllFunctionsProvider(const QString& dllPath)
{
	if (!dllPath.isEmpty()){
		m_handler = ::LoadLibraryW(dllPath.toStdWString().c_str());
	}
}


CDllFunctionsProvider::~CDllFunctionsProvider()
{
	Reset();
}


void CDllFunctionsProvider::Reset()
{
	if (m_handler != NULL){
		::FreeLibrary(m_handler);
	}

	m_handler = NULL;
}


bool CDllFunctionsProvider::OpenDll(const QString& dllPath)
{
	Reset();

	m_handler = ::LoadLibraryW(dllPath.toStdWString().c_str());

	return IsValid();
}


// reimplemented (isys::IFunctionsProvider)

bool CDllFunctionsProvider::IsValid() const
{
	return m_handler != NULL;
}


void* CDllFunctionsProvider::GetFunction(const std::string& id) const
{
	if (IsValid()){
		return (void*)::GetProcAddress(m_handler, id.c_str());
	}
	else{
		return NULL;
	}
}


} // namespace iwin


