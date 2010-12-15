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


I_EXPORT_PACKAGE(
			"IacfQt",
			"Imaging tools Qt-based general package",
			"Imaging Tools Qt Standard" IM_PROJECT("IACF") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));


I_EXPORT_COMPONENT(
			SamplingParamsGui,
			"Sampling parameters",
			"Interval Sample Sampling Model Parameters GUI Qt");

I_EXPORT_COMPONENT(
			SampleAcquisitionGui,
			"Sample acquisition GUI",
			"Sample Sampling Acquisition GUI");

I_EXPORT_COMPONENT(
			ScriptDataProcessor,
			"Test sample acquisition based on QtScript",
			"Test Sample Sampling Acquisition Script QtScript");

I_EXPORT_COMPONENT(
			InspectionTaskGui,
			"GUI for general inspection task",
			"Task Supplier Composite Observer" IM_TAG("Inspection GUI Editor"));

I_EXPORT_COMPONENT(
			GeneralSupplierGui,
			"General GUI for supplier (usable as subtask GUI)",
			"Task Supplier Observer Subtask" IM_TAG("Inspection GUI Editor"));


} // namespace IacfQtPck


