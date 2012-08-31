#ifndef iwin_CSystem_included
#define iwin_CSystem_included


// Qt includes
#include <QtCore/QString>

namespace iwin
{


/**
	Windows specific services

	\ingroup Windows
*/
class CSystem
{
public:
	/**
		Get path of the executable module.
	*/
	static QString GetModulePath(bool useApplicationModule = false, bool onlyDirectory = false);
};


} // namespace iwin


#endif // !iwin_CSystem_included


