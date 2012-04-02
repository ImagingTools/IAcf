#include "iwin/CDefaultServicesProvider.h"


// Qt includes
#include <QtCore/QDateTime>

#include "istd/TSingleFactory.h"
#include "istd/CStaticServicesProvider.h"

#include "iwin/CProcessEnvironment.h"


namespace iwin
{


void CDefaultServicesProvider::RegisterServices()
{
	static iwin::CProcessEnvironment applicationEnvironment;
	istd::CStaticServicesProvider::RegisterService<isys::IProcessEnvironment>(&applicationEnvironment);

	static istd::TSingleFactory<QDateTime, QDateTime> dateTimeFactory("");
	istd::CStaticServicesProvider::RegisterFactory(&dateTimeFactory);
}


} // namespace iwin


