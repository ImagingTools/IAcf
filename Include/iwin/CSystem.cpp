#include "iwin/CSystem.h"


// Windows includes
#include <windows.h>


// Qt includes
#include <QtCore/QStringList>


EXTERN_C IMAGE_DOS_HEADER __ImageBase;


namespace iwin
{


// public methods

QString CSystem::GetModulePath(bool useApplicationModule, bool onlyDirectory)
{
	WCHAR moduleFileName[MAX_PATH] = {0};

	HINSTANCE moduleBasePtr = useApplicationModule ? NULL : (HINSTANCE)&__ImageBase;

	::GetModuleFileNameW(moduleBasePtr, moduleFileName, MAX_PATH);

	if (onlyDirectory){
		for (int i = MAX_PATH - 1; i >= 0; i--)   {   
			if (moduleFileName[i] == '\\' || moduleFileName[i] == '/'){   
				moduleFileName[i] = '\0';
				break;
			}
		}   	
	}

	return QString::fromStdWString(moduleFileName);
}


} // namespace iwin


