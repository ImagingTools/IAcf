#include "iwin/CProcessEnvironment.h"


// Windows includes
#include <windows.h>


// Qt includes
#include <QtCore/QStringList>


EXTERN_C IMAGE_DOS_HEADER __ImageBase;


namespace iwin
{


// public methods
	
// reimplemented (isys::IProcessEnvironment)

int CProcessEnvironment::GetMainThreadId() const
{
	return ::GetCurrentThreadId();
}


void CProcessEnvironment::Sleep(double seconds)
{
	::Sleep(DWORD(seconds * 1000));
}


QString CProcessEnvironment::GetTempDirPath() const
{
	wchar_t tempPath[MAX_PATH] = {0};
	::GetTempPathW(MAX_PATH, tempPath);

	int length = wcslen(tempPath);

	if (tempPath[length - 1] == '\\'){
		tempPath[length - 1] = '\0';
	}

	return QString::fromStdWString(tempPath);
}


QString CProcessEnvironment::GetWorkingDirectory() const
{
	wchar_t workingDirectory[MAX_PATH] = {0};

	if (::GetCurrentDirectoryW(MAX_PATH, workingDirectory) != 0){
		return QString::fromStdWString(workingDirectory);
	}

	return QString();
}


QStringList CProcessEnvironment::GetApplicationArguments() const
{
	QStringList applicationArguments;

	LPWSTR commandLinePtr = GetCommandLineW();
	if (commandLinePtr != NULL){
		int argumentsCount = 0;
		LPWSTR* applicationArgumentsPtr = CommandLineToArgvW(commandLinePtr, &argumentsCount);
		if (applicationArgumentsPtr != NULL){
			for (int argumentIndex = 0; argumentIndex < argumentsCount; argumentIndex++){
				applicationArguments.push_back(QString::fromStdWString(applicationArgumentsPtr[argumentIndex]));
			}
		}
	}

	return applicationArguments;
}


QString CProcessEnvironment::GetModulePath(bool useApplicationModule /*= false*/, bool onlyDirectory /*= false*/) const
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


CProcessEnvironment::EnvironmentVariables CProcessEnvironment::GetEnvironmentVariables() const
{
	EnvironmentVariables environmentVariables;

	LPWCH environmentStringsPtr = GetEnvironmentStringsW();
	if (environmentStringsPtr != NULL){
		LPCWSTR lpszVariable = (LPCWSTR)environmentStringsPtr;
		while (*lpszVariable){
			QString varPair = QString::fromStdWString(lpszVariable);
			int separatorIndex = varPair.lastIndexOf('=');
			if (separatorIndex != -1){
				QString variableName = varPair.left(separatorIndex);
				QString variableValue = varPair.mid(separatorIndex + 1, varPair.length());
			
				environmentVariables[variableName] = variableValue;
			}

			lpszVariable += lstrlenW(lpszVariable) + 1;
		}

		FreeEnvironmentStringsW(environmentStringsPtr);
	}

	return environmentVariables;
}


void CProcessEnvironment::SetEnvironmentVariableValue(const QString& variableName, const QString& value)
{
	::SetEnvironmentVariable(variableName.toStdString().c_str(), value.toStdString().c_str());
}


} // namespace iwin


