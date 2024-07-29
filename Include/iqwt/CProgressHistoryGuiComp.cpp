#include <iqwt/CProgressHistoryGuiComp.h>


// Qt includes
#include <QtCore/QCoreApplication>
#include <QtGui/QPen>

// Qwt includes
#include <qwt_plot_layout.h>


namespace iqwt
{


CProgressHistoryGuiComp::CProgressHistoryGuiComp()
{
}


// reimplemented (ibase::CCumulatedProgressManagerBase)

void CProgressHistoryGuiComp::OpenTask(TaskBase* taskPtr, const TaskInfo& taskInfo, double weight, bool isCancelable)
{
	BaseClass2::OpenTask(taskPtr, taskInfo, weight, isCancelable);

	SessionPtr& sessionPtr = m_idToSessionMap[taskPtr];

	sessionPtr.SetPtr(new Session);

	// setup current plot curve:
	sessionPtr->axisY.clear();
	sessionPtr->curve.setRenderHint(QwtPlotItem::RenderAntialiased);
	sessionPtr->curve.setTitle(taskInfo.description);
	sessionPtr->curve.setSamples(&m_axisXData[0], &m_axisXData[0], 0);
	if (m_plotPtr.IsValid()) {
		sessionPtr->curve.attach(m_plotPtr.GetPtr());
	}

	QColor lineColor = Qt::GlobalColor(Qt::red + int(qHash(taskPtr)) % (Qt::transparent - Qt::red));

	int colorsCount = qMin(m_progressIdsAttrPtr.GetCount(), m_progressColorsAttrPtr.GetCount());
	for (int i = 0; i < colorsCount; ++i) {
		if (m_progressIdsAttrPtr[i] == taskInfo.id) {
			lineColor = QColor(m_progressColorsAttrPtr[i]);
		}
	}

	sessionPtr->curve.setPen(QPen(lineColor, 2));
	sessionPtr->isCancelable = isCancelable;
	sessionPtr->taskInfo = taskInfo;

	UpdateState();
}


void CProgressHistoryGuiComp::CloseTask(TaskBase* taskPtr)
{
	BaseClass2::CloseTask(taskPtr);

	IdToSessionMap::iterator foundIter = m_idToSessionMap.find(taskPtr);
	if (foundIter == m_idToSessionMap.end()) {
		return;
	}

	SessionPtr& sessionPtr = foundIter.value();
	Q_ASSERT(sessionPtr.IsValid());

	if (m_plotPtr.IsValid()) {
		sessionPtr->curve.detach();
	}

	m_idToSessionMap.erase(foundIter);

	UpdateState();
}


void CProgressHistoryGuiComp::ReportTaskProgress(TaskBase* taskPtr, double progress)
{
	BaseClass2::ReportTaskProgress(taskPtr, progress);

	IdToSessionMap::iterator foundIter = m_idToSessionMap.find(taskPtr);
	if (foundIter == m_idToSessionMap.end()) {
		return;
	}

	SessionPtr& sessionPtr = foundIter.value();

	sessionPtr->axisY.insert(sessionPtr->axisY.begin(), progress * 100);
	while (sessionPtr->axisY.size() > m_axisXData.size()) {
		sessionPtr->axisY.pop_back();
	}

	Q_ASSERT(sessionPtr->axisY.size() <= m_axisXData.size());
	sessionPtr->curve.setSamples(&m_axisXData[0], &sessionPtr->axisY[0], int(sessionPtr->axisY.size()));

	if (m_plotPtr.IsValid()) {
		emit m_plotPtr->update();

		QCoreApplication::processEvents();
	}
}


// protected methods

void CProgressHistoryGuiComp::UpdateState()
{
	if (!IsGuiCreated()){
		return;
	}

	if (IsCancelable()){
		CancelButton->setEnabled(true);
	}
	else{
		CancelButton->setEnabled(false);
		CancelButton->setChecked(false);
	}


	int sessionsCount = int(m_idToSessionMap.size());
	if (sessionsCount == 0){
		DescriptionLabel->setText(tr("Finished"));
	}
	else if (sessionsCount == 1){
		SessionPtr& sessionPtr = m_idToSessionMap.begin().value();
		Q_ASSERT(sessionPtr.IsValid());

		DescriptionLabel->setText(sessionPtr->taskInfo.description);
	}
	else{
		DescriptionLabel->setText(tr("%1 Sessions").arg(sessionsCount));
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CProgressHistoryGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QWidget* widgetPtr = GetWidget();
	if (widgetPtr == NULL){
		return;
	}
	m_plotPtr.SetPtr(new QwtPlot(DiagramFrame));
	m_plotPtr->setLineWidth(1);
	m_plotPtr->setAxisScale(QwtPlot::yLeft, 0, 100);
	m_plotPtr->setAxisScale(QwtPlot::xBottom, *m_historyStepsCountAttrPtr, 0);
	m_plotPtr->plotLayout()->setAlignCanvasToScales(true);
	m_plotPtr->setAutoReplot(true);
	m_plotPtr->setEnabled(false);

	// setup the plot grid:
	m_gridPtr.SetPtr(new QwtPlotGrid());
	QPen gridPen(QBrush(Qt::gray), 0, Qt::DashLine);
	m_gridPtr->setMajorPen(gridPen);
	m_gridPtr->attach(m_plotPtr.GetPtr());

	m_nowMarkerPtr.SetPtr(new QwtPlotMarker());
	m_nowMarkerPtr->setValue(0.0, 0.0);
	m_nowMarkerPtr->setLineStyle(QwtPlotMarker::VLine);
	m_nowMarkerPtr->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
	m_nowMarkerPtr->setLinePen(QPen(Qt::darkMagenta, 2, Qt::DotLine));
	m_nowMarkerPtr->attach(m_plotPtr.GetPtr());

	// add plot view to frame layout
	QLayout* layoutPtr = DiagramFrame->layout();
	if (layoutPtr != NULL){
		layoutPtr->addWidget(m_plotPtr.GetPtr());
	}

	DescriptionLabel->setVisible(*m_showTaskDescriptionAttrPtr);
	CancelButton->setVisible(*m_showCancelAttrPtr);
	StatusFrame->setVisible(*m_showTaskDescriptionAttrPtr || *m_showCancelAttrPtr);

	// attach all open sessions
	for (		IdToSessionMap::ConstIterator sessionIter = m_idToSessionMap.constBegin();
				sessionIter != m_idToSessionMap.constEnd();
				++sessionIter){
		const SessionPtr& sessionPtr = *sessionIter;

		sessionPtr->curve.attach(m_plotPtr.GetPtr());
	}

	UpdateState();
}


void CProgressHistoryGuiComp::OnGuiDestroyed()
{
	m_nowMarkerPtr->detach();
	m_nowMarkerPtr.Reset();

	m_gridPtr->detach();
	m_gridPtr.Reset();

	// detach all open sessions
	for (		IdToSessionMap::ConstIterator sessionIter = m_idToSessionMap.constBegin();
				sessionIter != m_idToSessionMap.constEnd();
				++sessionIter){
		const SessionPtr& sessionPtr = *sessionIter;

		sessionPtr->curve.detach();
	}

	m_plotPtr.Reset();

	BaseClass::OnGuiDestroyed();
}


// reimplemented (icomp::CComponentBase)

void CProgressHistoryGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_axisXData.clear();

	int stepsCount = *m_historyStepsCountAttrPtr;
	for (int i = 0; i <= stepsCount; ++i){
		m_axisXData.push_back(i);
	}
}


// protected slots

void CProgressHistoryGuiComp::on_CancelButton_clicked()
{
	SetCanceled();
}


} // namespace iqwt


