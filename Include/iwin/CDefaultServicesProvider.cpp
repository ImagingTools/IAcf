#include "iwin/CDefaultServicesProvider.h"


// Qt includes
#include <QtCore/QDateTime>

#include "istd/TSingleFactory.h"
#include "istd/CStaticServicesProvider.h"

#include "iwin/CProcessEnvironment.h"
#include "iwin/CSystemInfo.h"


namespace iwin
{


void CDefaultServicesProvider::RegisterServices()
{
	static iwin::CProcessEnvironment applicationEnvironment;
	istd::CStaticServicesProvider::RegisterService<isys::IProcessEnvironment>(&applicationEnvironment);

	static istd::TSingleFactory<QDateTime, QDateTime> dateTimeFactory("");
	istd::CStaticServicesProvider::RegisterFactory(&dateTimeFactory);

	static istd::TSingleFactory<isys::ISystemInfo, iwin::CSystemInfo> systemInfoFactory("");
	istd::CStaticServicesProvider::RegisterFactory(&systemInfoFactory);
}


} // namespace iwin


