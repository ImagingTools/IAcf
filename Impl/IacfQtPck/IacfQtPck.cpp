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

I_EXPORT_COMPONENT(ProgressManagerGui, "Progress manager GUI using progress bar widget to visualize progress", "Progress Manager Qt GUI");
I_EXPORT_COMPONENT(AcquisitionProcessorGui, "Processor acquisition GUI", "Acquisition Processor Parameter GUI View");

I_EXPORT_COMPONENT(SamplingParamsGui, "Sampling parameters", "Interval Sample Sampling Model Parameters GUI Qt");
I_EXPORT_COMPONENT(SampleAcquisitionGui, "Sample acquisition GUI", "Sample Sampling Acquisition GUI Qt");
I_EXPORT_COMPONENT(ScriptSampleAcquisition, "Test sample acquisition based on QtScript", "Test Sample Sampling Acquisition Script QtScript Qt");


} // namespace IacfQtPck


