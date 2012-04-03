#include "iwin/CDefaultServicesProvider.h"


// Qt includes
#include <QtCore/QDateTime>

#include "istd/TSingleFactory.h"
#include "istd/CStaticServicesProvider.h"


namespace iwin
{


void CDefaultServicesProvider::RegisterServices()
{
	static istd::TSingleFactory<QDateTime, QDateTime> dateTimeFactory("");
	istd::CStaticServicesProvider::RegisterFactory(&dateTimeFactory);
}


} // namespace iwin


