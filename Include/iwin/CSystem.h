// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-IAcf-Commercial
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

	/**
		Enable LFH (Low Fragmentation Heap).
	*/
	static bool EnableLowFragmentationHeap();
};


} // namespace iwin


#endif // !iwin_CSystem_included


