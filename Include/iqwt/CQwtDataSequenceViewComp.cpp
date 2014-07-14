#include "iqwt/CQwtDataSequenceViewComp.h"


// ACF includes
#include "imath/CVarVector.h"

// Qwt includes
#include "qwt_plot_grid.h"
#include "qwt_scale_draw.h"


namespace iqwt
{


// public methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CQwtDataSequenceViewComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	int channelsCount = 0;

	imeas::IDataSequence* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		QStringList channelNames;
		const imeas::IDataSequenceInfo* sequenceInfoPtr = objectPtr->GetSequenceInfo();
		
		channelsCount = objectPtr->GetChannelsCount();

		for (int channelIndex = 0; channelIndex < channelsCount; channelIndex++){
			if (sequenceInfoPtr != NULL){
				channelNames << sequenceInfoPtr->GetNumericValueName(channelIndex);
			}
			else{
				channelNames << tr("Channel %1").arg(channelIndex + 1);
			}
		}

		if (objectPtr->GetChannelsCount() != m_channelCurves.GetCount()){
			ClearPlot();

			ChannelCombo->clear();

			ChannelCombo->addItem(tr("All Channels"));

			for (int channelIndex = 0; channelIndex < channelsCount; channelIndex++){
				QwtPlotCurve* curvePtr = new QwtPlotCurve();

				curvePtr->setPen(QPen(Qt::GlobalColor(Qt::red + channelIndex)));
				
				curvePtr->setStyle(QwtPlotCurve::Steps);
			
				curvePtr->attach(m_plotPtr.GetPtr());
				m_channelCurves.PushBack(curvePtr);

				ChannelCombo->addItem(channelNames[channelIndex]);
			}
		}

		if (m_verticalMarkers.GetCount() != m_verticalLinesCompPtr.GetCount() || m_horizontalMarkers.GetCount() != m_horizontalLinesCompPtr.GetCount()) {
			ClearMarkers();

			int verticalCount = m_verticalLinesCompPtr.GetCount();
			for (int verticalIndex = 0; verticalIndex < verticalCount; ++verticalIndex) {
				imeas::INumericValue* value = m_verticalLinesCompPtr[verticalIndex];
				if (value != NULL) {
					imath::CVarVector values = value->GetValues();
					int valueCount = values.GetElementsCount();
					for (int valueIndex = 0; valueIndex < valueCount; ++valueIndex) {
						double XVal = values.GetElement(valueIndex);
						QString name = value->GetNumericConstraints()->GetNumericValueName(valueIndex);
						QwtPlotMarker* marker = new QwtPlotMarker();
						marker->setLabel(QwtText(name));
						marker->setLabelOrientation(Qt::Vertical);
						marker->setLabelAlignment(Qt::AlignBottom | Qt::AlignLeft);
						//marker->setSpacing(50);
						//marker->setLinePen(QPen(Qt::GlobalColor(colorIndex++)));
						marker->setLineStyle(QwtPlotMarker::VLine);
						marker->setXValue(XVal);
						marker->attach(m_plotPtr.GetPtr());
						m_verticalMarkers.PushBack(marker);
					}
				}
			}

			int horizontalCount = m_horizontalLinesCompPtr.GetCount();
			for (int horizontalIndex = 0; horizontalIndex < horizontalCount; ++horizontalIndex) {
				imeas::INumericValue* value = m_horizontalLinesCompPtr[horizontalIndex];
				if (value != NULL) {
					imath::CVarVector values = value->GetValues();
					int valueCount = values.GetElementsCount();
					for (int valueIndex = 0; valueIndex < valueCount; ++valueIndex) {
						double YVal = values.GetElement(valueIndex);
						QString name = value->GetNumericConstraints()->GetNumericValueName(valueIndex);
						QwtPlotMarker* marker = new QwtPlotMarker();
						marker->setLabel(QwtText(name));
						//marker->setLinePen(QPen(Qt::GlobalColor(colorIndex++)));
						marker->setLineStyle(QwtPlotMarker::HLine);
						marker->setYValue(YVal);
						marker->attach(m_plotPtr.GetPtr());
						m_horizontalMarkers.PushBack(marker);
					}
				}
			}
		}
		
		double maxValue = 0.0;
		double minValue = 0.0;

		for (int channelIndex = 0; channelIndex < channelsCount; channelIndex++){
			int samplesCount = objectPtr->GetSamplesCount();
	
			QVector<double> xData(samplesCount);
			QVector<double> yData(samplesCount);

			for (int sampleIndex = 0; sampleIndex < samplesCount; sampleIndex++){
				double sample = objectPtr->GetSample(sampleIndex, channelIndex);

				xData[sampleIndex] = sampleIndex;
				yData[sampleIndex] = sample;

				if (sample > maxValue){
					maxValue = sample;
				}

				if (sample < minValue){
					minValue = sample;
				}
			}

			QwtPlotCurve* curvePtr = m_channelCurves.GetAt(channelIndex);
			Q_ASSERT(curvePtr != NULL);

			maxValue = qCeil(maxValue * 100) / 100.0;

			curvePtr->setSamples(xData, yData);

			m_plotPtr->setAxisScale(QwtPlot::xBottom, 0.0, samplesCount, 16); 
			m_plotPtr->setAxisMaxMinor(QwtPlot::xBottom, 4);

			m_plotPtr->setAxisScale(QwtPlot::yLeft, minValue, maxValue, (maxValue-minValue) / 10); 
			m_plotPtr->setAxisMaxMinor(QwtPlot::yLeft, 5);

			if (channelIndex != ChannelCombo->currentIndex() && ChannelCombo->currentIndex() != 0){
				curvePtr->setVisible(false);
			}
		}
	}
	else{
		ClearPlot();
		ClearMarkers();
		
		ChannelCombo->clear();
	}

	if (IsGuiCreated()){
		ChannelSelectorFrame->setVisible(channelsCount > 1);
	}
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CQwtDataSequenceViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	m_plotPtr.SetPtr(new QwtPlot(PlotFrame));
	m_plotPtr->setLineWidth(0);
	m_plotPtr->setAxisFont(QwtPlot::xBottom, qApp->font());
	m_plotPtr->setAxisFont(QwtPlot::yLeft, qApp->font());
	m_plotPtr->setAutoReplot(true);

	QwtPlotPicker* plotPickerPtr = new DataSequencePlotPicker(*this, QwtPlot::xBottom, QwtPlot::yLeft, m_plotPtr->canvas());
	plotPickerPtr->setTrackerMode(QwtPicker::AlwaysOn);
	plotPickerPtr->setEnabled(true);

	QwtPlotGrid* plotGridPtr = new QwtPlotGrid;
	plotGridPtr->enableXMin(true);
	plotGridPtr->enableYMin(true);
	plotGridPtr->setMajorPen(QPen(Qt::black, 0, Qt::DotLine));
	plotGridPtr->setMinorPen(QPen(Qt::gray, 0 , Qt::DotLine));
	plotGridPtr->attach(m_plotPtr.GetPtr());

	QLayout* layoutPtr = PlotFrame->layout();
	if (layoutPtr == NULL){
		layoutPtr = new QVBoxLayout(PlotFrame);
		layoutPtr->setMargin(0);
	}

	layoutPtr->addWidget(m_plotPtr.GetPtr());
}


void CQwtDataSequenceViewComp::OnGuiDestroyed()
{
	ClearPlot();
	ClearMarkers();

	m_plotPtr.Reset();

	BaseClass::OnGuiDestroyed();
}


// private slots

void CQwtDataSequenceViewComp::on_ChannelCombo_currentIndexChanged(int index)
{
	for (int channelIndex = 0; channelIndex < m_channelCurves.GetCount(); channelIndex++){
		m_channelCurves.GetAt(channelIndex)->setVisible((channelIndex+1 == index)||(index == 0));
	}
}


// private methods

void CQwtDataSequenceViewComp::ClearPlot()
{
	if (IsGuiCreated()){
		for (int curveIndex = 0; curveIndex < m_channelCurves.GetCount(); curveIndex++){
			m_channelCurves.GetAt(curveIndex)->detach();
		}
		
		m_channelCurves.Reset();
	}
}


void CQwtDataSequenceViewComp::ClearMarkers()
{
	if (IsGuiCreated()){
		for (int markerIndex = 0; markerIndex < m_horizontalMarkers.GetCount(); markerIndex++){
			m_horizontalMarkers.GetAt(markerIndex)->detach();
		}
		for (int markerIndex = 0; markerIndex < m_verticalMarkers.GetCount(); markerIndex++){
			m_verticalMarkers.GetAt(markerIndex)->detach();
		}

		m_horizontalMarkers.Reset();
		m_verticalMarkers.Reset();
	}
}



// public methods of the embedded class DataSequencePlotPicker

CQwtDataSequenceViewComp::DataSequencePlotPicker::DataSequencePlotPicker(CQwtDataSequenceViewComp& parent, int xAxis, int yAxis, QWidget* canvasPtr)
	:BaseClass(xAxis, yAxis, canvasPtr),
	m_parent(parent)
{
}


// reimplemented (QwtPlotPicker)

QwtText CQwtDataSequenceViewComp::DataSequencePlotPicker::trackerText(const QPoint& position) const
{
	imeas::IDataSequence* objectPtr = m_parent.GetObjectPtr();
	if (objectPtr == NULL){
		return QwtText();
	}

	if (objectPtr->IsEmpty()){
		return QwtText();
	}

	Q_ASSERT(m_parent.IsGuiCreated());

	int sampleIndex = int(invTransform(position).x() + 0.5);

	if (sampleIndex >= objectPtr->GetSamplesCount()){
		return QwtText();
	}

	if (sampleIndex < 0){
		return QwtText();
	}

	double positionY = (double)position.y();

	int currentCurveIndex = m_parent.ChannelCombo->currentIndex();
	double sample = 0.0;

	if (currentCurveIndex == 0) {
		double distance = DBL_MAX;
		int channelsCount = objectPtr->GetChannelsCount();
		for (int channelsIndex = 0;channelsIndex < channelsCount; ++channelsIndex) {
			double currentSample = objectPtr->GetSample(sampleIndex, channelsIndex);
			double diff = fabs(currentSample-positionY);
			if (diff < distance) {
				distance = diff;
				sample = currentSample;
			}
		}
	}
	else{
		sample = objectPtr->GetSample(sampleIndex, currentCurveIndex-1);
	}

	QString text = QString("%1: %2").arg(sampleIndex).arg(sample);

	return text;
}


} // namespace iqwt


