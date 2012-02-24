#ifndef iwin_CProcessEnvironment_included
#define iwin_CProcessEnvironment_included


#include "isys/IProcessEnvironment.h"


namespace iwin
{


/**
	Windows based implementation of interface \c isys::IProcessEnvironment.

	\ingroup Windows
*/
class CProcessEnvironment: virtual public isys::IProcessEnvironment
{
public:
	// reimplemented (isys::IProcessEnvironment)
	virtual int GetMainThreadId() const;
	virtual void Sleep(double seconds);
	virtual QString GetTempDirPath() const;
	virtual QString GetWorkingDirectory() const;
	virtual QStringList GetApplicationArguments() const;
	virtual QString GetModulePath(bool useApplicationModule = false, bool onlyDirectory = false) const;
	virtual EnvironmentVariables GetEnvironmentVariables() const;
	virtual void SetEnvironmentVariableValue(const QString& variableName, const QString& value);
};


} // namespace iwin


#endif // !iwin_CProcessEnvironment_included
