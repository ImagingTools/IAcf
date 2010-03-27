#include <QDir>

#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "IacfQtPck.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
	}

} instance;


namespace IacfQtPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE("IacfQt", "Imaging tools Qt-based general package", "Imaging Tools Qt Standard");

I_EXPORT_COMPONENT(
			SamplingParamsGui,
			"Sampling parameters",
			"Interval Sample Sampling Model Parameters GUI Qt",
			icomp::IComponentStaticInfo::CCT_GUI | icomp::IComponentStaticInfo::CCT_DATA_PRESENTATION);

I_EXPORT_COMPONENT(
			SampleAcquisitionGui,
			"Sample acquisition GUI",
			"Sample Sampling Acquisition GUI Qt",
			icomp::IComponentStaticInfo::CCT_GUI);

I_EXPORT_COMPONENT(
			ScriptSampleAcquisition,
			"Test sample acquisition based on QtScript",
			"Test Sample Sampling Acquisition Script QtScript Qt",
			icomp::IComponentStaticInfo::CCT_DATA_PROCESSING);


} // namespace IacfQtPck


