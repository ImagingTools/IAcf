#include <iqwt/CProgressHistoryGuiComp.h>


// Qt includes
#include <QtCore/QCoreApplication>
#include <QtGui/QPen>

// Qwt includes
#include <qwt_plot_layout.h>


namespace iqwt
{


CProgressHistoryGuiComp::CProgressHistoryGuiComp()
:	m_currentId(0),
	m_cancelsCount(0)
{
}


// reimplemented (ibase::IProgressManager)

int CProgressHistoryGuiComp::BeginProgressSession(
			const QByteArray& progressId,
			const QString& description,
			bool isCancelable)
{
	if (m_idToSessionMap.isEmpty()){
		m_currentId = 0;
	}

	int sessionId = m_currentId;

	SessionPtr& sessionPtr = m_idToSessionMap[sessionId];
	if (sessionPtr.IsValid()){
		return -1;	// we cannot create new session, ID is used yet
	}

	m_currentId = (m_currentId + 1) & 0x7fffffff;

	sessionPtr.SetPtr(new Session);

	// setup current plot curve:
	sessionPtr->axisY.clear();
	sessionPtr->curve.setRenderHint(QwtPlotItem::RenderAntialiased);
	sessionPtr->curve.setTitle(description);
	sessionPtr->curve.setSamples(&m_axisXData[0], &m_axisXData[0], 0);
	if (m_plotPtr.IsValid()){
		sessionPtr->curve.attach(m_plotPtr.GetPtr());
	}

	QColor lineColor = Qt::GlobalColor(Qt::red + int(qHash(progressId)) % (Qt::transparent - Qt::red));

	int colorsCount = qMin(m_progressIdsAttrPtr.GetCount(), m_progressColorsAttrPtr.GetCount());
	for (int i = 0; i < colorsCount; ++i){
		if (m_progressIdsAttrPtr[i] == progressId){
			lineColor = QColor(m_progressColorsAttrPtr[i]);
		}
	}

	sessionPtr->curve.setPen(QPen(lineColor, 2));
	sessionPtr->isCancelable = isCancelable;
	sessionPtr->description = description;

	if (sessionPtr->isCancelable){
		++m_cancelsCount;
	}

	UpdateState();

	return sessionId;
}


void CProgressHistoryGuiComp::EndProgressSession(int sessionId)
{
	IdToSessionMap::iterator foundIter = m_idToSessionMap.find(sessionId);
	if (foundIter == m_idToSessionMap.end()){
		return;
	}

	SessionPtr& sessionPtr = foundIter.value();
	Q_ASSERT(sessionPtr.IsValid());

	sessionPtr->curve.setPen(QPen(Qt::GlobalColor(Qt::cyan + sessionId), 0.5));

	if (sessionPtr->isCancelable){
		--m_cancelsCount;
		Q_ASSERT(m_cancelsCount >= 0);	// number of all cancelable sessions cannot be negative
	}

	if (m_plotPtr.IsValid()){
		sessionPtr->curve.detach();
	}

	m_idToSessionMap.erase(foundIter);

	UpdateState();
}


void CProgressHistoryGuiComp::OnProgress(int sessionId, double currentProgress)
{
	IdToSessionMap::iterator foundIter = m_idToSessionMap.find(sessionId);
	if (foundIter == m_idToSessionMap.end()){
		return;
	}

	SessionPtr& sessionPtr = foundIter.value();

	sessionPtr->axisY.insert(sessionPtr->axisY.begin(), currentProgress * 100);
	while (sessionPtr->axisY.size() > m_axisXData.size()){
		sessionPtr->axisY.pop_back();
	}

	Q_ASSERT(sessionPtr->axisY.size() <= m_axisXData.size());
	sessionPtr->curve.setSamples(&m_axisXData[0], &sessionPtr->axisY[0], int(sessionPtr->axisY.size()));

	if (m_plotPtr.IsValid()){
		emit m_plotPtr->update();

		QCoreApplication::processEvents();
	}
}


bool CProgressHistoryGuiComp::IsCanceled(int /*sessionId*/) const
{
	return IsGuiCreated() && CancelButton->isChecked();
}


// protected methods

void CProgressHistoryGuiComp::UpdateState()
{
	if (!IsGuiCreated()){
		return;
	}

	if (m_cancelsCount > 0){
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

		DescriptionLabel->setText(sessionPtr->description);
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


} // namespace iqwt


