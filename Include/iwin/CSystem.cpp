#include <iwin/CSystem.h>


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


bool CSystem::EnableLowFragmentationHeap()
{
#if defined(_MSC_VER)
	bool result = true;

	ULONG enableLFH = 2;
	HANDLE heaps[1025];
	DWORD nheaps = GetProcessHeaps((sizeof(heaps) / sizeof(HANDLE)) - 1, heaps);
	for (DWORD i = 0; i < nheaps; ++i) {
		BOOL ok = HeapSetInformation(heaps[i], HeapCompatibilityInformation, &enableLFH, sizeof(enableLFH));
		if (ok == FALSE){
			result = false;

			printf("Low Fragmentation failed for Heap %x\n", heaps[i]);
		}
		else{
			printf("Low Fragmentation enabled for Heap %x\n", heaps[i]);
		}
	}

	return result;
#endif

	return false;
}


} // namespace iwin


