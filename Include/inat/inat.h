// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-IAcf-Commercial
#ifndef inat_included
#define inat_included


#ifdef _WIN32
#include <iwin/iwin.h>
#else
#include <iqt/iqt.h>
#endif


/**
	This namespace contains implementations for the compile-time native plattform
*/
namespace inat
{


#ifdef _WIN32
	using namespace iwin;
#else
	using namespace iqt;
#endif


} // namespace inat


#endif // !inat_included
