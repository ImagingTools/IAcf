#include <QDir>

#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "IacfQtPck.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtdoc);
	}

} instance;


namespace IacfQtPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE("IacfQt", "Imaging tools Qt-based general package", "Imaging Tools Qt Standard");

I_EXPORT_COMPONENT(SamplingParamsGui, "Sampling parameters", "Interval Sample Sampling Model Parameters GUI Qt");
I_EXPORT_COMPONENT(SampleAcquisitionGui, "Sample acquisition GUI", "Sample Sampling Acquisition GUI Qt");
I_EXPORT_COMPONENT(ScriptSampleAcquisition, "Test sample acquisition based on QtScript", "Test Sample Sampling Acquisition Script QtScript Qt");


} // namespace IacfQtPck


