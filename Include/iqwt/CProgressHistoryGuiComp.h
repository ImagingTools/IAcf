#ifndef iqwt_CProgressHistoryGuiComp_included
#define iqwt_CProgressHistoryGuiComp_included


// Qwt includes
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_grid.h>

// ACF includes
#include <ibase/CCumulatedProgressManagerBase.h>
#include <iqtgui/TDesignerGuiCompBase.h>

#include <GeneratedFiles/iqwt/ui_CProgressHistoryGuiComp.h>


namespace iqwt
{



/**
	Shows progress history for nonlinear and 'non-progressive' processes.
*/
class CProgressHistoryGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CProgressHistoryGuiComp>,
			public ibase::CCumulatedProgressManagerBase
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CProgressHistoryGuiComp> BaseClass;
	typedef ibase::CCumulatedProgressManagerBase BaseClass2;

	I_BEGIN_COMPONENT(CProgressHistoryGuiComp);
		I_REGISTER_INTERFACE(ibase::IProgressManager);
		I_ASSIGN(m_historyStepsCountAttrPtr, "HistoryStepsCount", "Number of steps shown in this history", true, 100);
		I_ASSIGN(m_showTaskDescriptionAttrPtr, "ShowTaskDescription", "If true, description of current task will be shown", true, true);
		I_ASSIGN(m_showCancelAttrPtr, "ShowCancel", "If true, cancel button will be shown", true, true);
		I_ASSIGN_MULTI_0(m_progressIdsAttrPtr, "ProgressIds", "List of known progress ID's", true);
		I_ASSIGN_MULTI_0(m_progressColorsAttrPtr, "ProgressColors", "Colors for known progress ID's,\nFormat: #RRGGBB or SVG color keyword names", true);
	I_END_COMPONENT;

	CProgressHistoryGuiComp();

	// reimplemented (ibase::CCumulatedProgressManagerBase)
	virtual void OpenTask(TaskBase* taskPtr, const TaskInfo& taskInfo, double weight, bool isCancelable) override;
	virtual void CloseTask(TaskBase* taskPtr) override;
	virtual void ReportTaskProgress(TaskBase* taskPtr, double progress, TaskStatus taskStatus) override;

protected:
	void UpdateState();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiDestroyed() override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

protected Q_SLOTS:
	void on_CancelButton_clicked();

private:
	I_ATTR(int, m_historyStepsCountAttrPtr);
	I_ATTR(bool, m_showTaskDescriptionAttrPtr);
	I_ATTR(bool, m_showCancelAttrPtr);
	I_MULTIATTR(QByteArray, m_progressIdsAttrPtr);
	I_MULTIATTR(QString, m_progressColorsAttrPtr);

	istd::TDelPtr<QwtPlot> m_plotPtr;
	istd::TDelPtr<QwtPlotMarker> m_nowMarkerPtr;
	istd::TDelPtr<QwtPlotGrid> m_gridPtr;

	typedef QVector<double> SingleData;

	struct Session
	{
		SingleData axisY;
		QwtPlotCurve curve;
		bool isCancelable;
		TaskInfo taskInfo;
	};

	typedef istd::TDelPtr<Session> SessionPtr;

	typedef QMap<TaskBase*, SessionPtr> IdToSessionMap;
	IdToSessionMap m_idToSessionMap;

	SingleData m_axisXData;
};


} // namespace iqwt


#endif // !iqwt_CProgressHistoryGuiComp_included


