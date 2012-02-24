#ifndef iwin_CDllFunctionsProvider_included
#define iwin_CDllFunctionsProvider_included


#include "iwin/iwin.h"


#include <windows.h>

#include "isys/IFunctionsProvider.h"

#include <QString>


namespace iwin
{


/**
	Provide access to public functions in DLL.

	\ingroup Windows
*/
class CDllFunctionsProvider: virtual public isys::IFunctionsProvider
{
public:
	/**
		Construct this object opening specified DLL.
		\param	dllPath	path to DLL or empty string if no DLL should be opened.
	 */
	CDllFunctionsProvider(const QString& dllPath = "");
	~CDllFunctionsProvider();

	bool OpenDll(const QString& dllPath);

	void Reset();

	// reimplemented (isys::IFunctionsProvider)
	virtual bool IsValid() const;
	virtual void* GetFunction(const std::string& id) const;

private:
	HINSTANCE m_handler;
};


} // namespace iwin


#endif // !iwin_CDllFunctionsProvider_included


