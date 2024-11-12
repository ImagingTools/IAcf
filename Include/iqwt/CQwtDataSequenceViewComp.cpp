#include <iqwt/CQwtDataSequenceViewComp.h>


// for DBL_MAX
#include <float.h>

// ACF includes
#include <imath/CVarVector.h>

// Qwt includes
#include <qwt_plot_grid.h>
#include <qwt_scale_draw.h>
#include <qwt_symbol.h>
#include <qwt_text.h>


namespace iqwt
{


// public methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CQwtDataSequenceViewComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	int channelsCount = 0;

	imeas::IDataSequence* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		QStringList channelNames;
		const imeas::IDataSequenceInfo* sequenceInfoPtr = objectPtr->GetSequenceInfo();
		
		channelsCount = objectPtr->GetChannelsCount();

		for (int channelIndex = 0; channelIndex < channelsCount; channelIndex++){
			if (sequenceInfoPtr != NULL){
				const iprm::IOptionsList& valueListInfo = sequenceInfoPtr->GetValueListInfo();

				channelNames << valueListInfo.GetOptionName(channelIndex);
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

				// Random color:
				QColor curveColor = Qt::GlobalColor(Qt::red + channelIndex);

				// Try to get staticaly defined color for the current channel:
				if (sequenceInfoPtr != NULL){
					const iprm::IOptionsList& valueListInfo = sequenceInfoPtr->GetValueListInfo();

					QByteArray channelId = valueListInfo.GetOptionId(channelIndex);

					int staticColorIndex = m_channelColorIdsAttrPtr.FindValue(channelId);
					if ((staticColorIndex >= 0) && (staticColorIndex < m_channelColorsAttrPtr.GetCount())){
						curveColor = QColor(m_channelColorsAttrPtr[staticColorIndex]);
					}
				}

				curvePtr->setPen(QPen(curveColor, 2, Qt::SolidLine));
				
				curvePtr->setStyle(GetQwtPlotStyle());
				QwtSymbol* pointSymbolPtr = new QwtSymbol(GetQwtSymbolStyle());

				curvePtr->setSymbol(pointSymbolPtr);
				curvePtr->attach(m_plotPtr.GetPtr());
				m_channelCurves.PushBack(curvePtr);

				ChannelCombo->addItem(channelNames[channelIndex]);
			}
		}

		int horizontalCount = 0;
		int verticalCount = 0;
		MakeValueLines(horizontalCount, false, m_horizontalLinesCompPtr.GetPtr(), m_horizontalMarkers);
		MakeValueLines(verticalCount, true, m_verticalLinesCompPtr.GetPtr(), m_verticalMarkers);

		int samplesCount = objectPtr->GetSamplesCount();
		double maxValue = 0.0;
		double minValue = 0.0;

		// Horizontal axis limits:
		double hMinValue = 0.0;
		double hMaxValue = samplesCount-1;
		MakeHorizontalAxisLimits(hMinValue, hMaxValue);

		// Fill data:
		for (int channelIndex = 0; channelIndex < channelsCount; channelIndex++){
			QVector<double> xData(samplesCount);
			QVector<double> yData(samplesCount);
			FillChannelData(xData, yData, minValue, maxValue, channelIndex, hMinValue, hMaxValue, objectPtr);

			QwtPlotCurve* curvePtr = m_channelCurves.GetAt(channelIndex);
			Q_ASSERT(curvePtr != NULL);

			curvePtr->setSamples(xData, yData);

			SetAxisLimits(hMinValue, hMaxValue, minValue, maxValue);
		}
	}
	else{
		ClearPlot();
		ClearMarkers(m_horizontalMarkers);
		ClearMarkers(m_verticalMarkers);
		
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
		layoutPtr->setContentsMargins(0,0,0,0);
	}

	layoutPtr->addWidget(m_plotPtr.GetPtr());
}


void CQwtDataSequenceViewComp::OnGuiDestroyed()
{
	ClearPlot();
	ClearMarkers(m_horizontalMarkers);
	ClearMarkers(m_verticalMarkers);

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


void CQwtDataSequenceViewComp::ClearMarkers(istd::TPointerVector<QwtPlotMarker>& markers)
{
	if (IsGuiCreated()){
		for (int markerIndex = 0; markerIndex < markers.GetCount(); markerIndex++){
			markers.GetAt(markerIndex)->detach();
		}
		markers.Reset();
	}
}


void CQwtDataSequenceViewComp::MakeHorizontalAxisLimits(double& minValue, double& maxValue) const
{
	if (m_verticalLinesCompPtr.IsValid()){
		imath::CVarVector verticalLinesValues = m_verticalLinesCompPtr->GetValues();
		const int verticalCount = verticalLinesValues.GetElementsCount();

		if (verticalCount > 1){
			int valuesCount = verticalLinesValues.GetElementsCount();
			minValue = verticalLinesValues.GetElement(0);
			maxValue = verticalLinesValues.GetElement(valuesCount - 1);

			if (minValue > maxValue){//swap
				double temp = minValue;
				minValue = maxValue;
				maxValue = temp;
			}
		}
	}
}


void CQwtDataSequenceViewComp::MakeValueLines(int& linesCount, const bool isVertical, const imeas::INumericValue* linesCompPtr, istd::TPointerVector<QwtPlotMarker>& markers)
{
	if (linesCompPtr != NULL){
		linesCount = linesCompPtr->GetValues().GetElementsCount();
	}

	if (markers.GetCount() != linesCount){
		ClearMarkers(markers);

		imath::CVarVector values = linesCompPtr->GetValues();
		for (int valueIndex = 0; valueIndex < linesCount; ++valueIndex){
			double axisVal = values.GetElement(valueIndex);

			QString name;

			const imeas::INumericConstraints* constraintsPtr = linesCompPtr->GetNumericConstraints();
			if (constraintsPtr != NULL){
				const iprm::IOptionsList& valueListInfo = constraintsPtr->GetValueListInfo();

				name = valueListInfo.GetOptionName(valueIndex);
			}
			else{
				name = QString("Channel-%1").arg(valueIndex + 1);
			}
			
			QwtPlotMarker* marker = new QwtPlotMarker();
			marker->setLabel(QwtText(name));

			if (isVertical){
				marker->setLabelOrientation(Qt::Vertical);
				marker->setLabelAlignment(Qt::AlignBottom | Qt::AlignLeft);
				marker->setLinePen(QPen(Qt::magenta, 2, Qt::DotLine));
				marker->setLineStyle(QwtPlotMarker::VLine);
			}
			else{
				marker->setLineStyle(QwtPlotMarker::HLine);
			}

			marker->setXValue(axisVal);
			marker->attach(m_plotPtr.GetPtr());
			markers.PushBack(marker);
		}
	}
}


/*
	if samples count fits the count of lines, it is assumed that datapoints lie exactly on these lines.
	otherwise if there are less lines than samples, the samples would be equidistantly drawn withing the range of Lines
	notice: if there were no lines provided, the range [hMinValue, hMaxValue] is set as [0, samplesCount -1] (see MakeHorizontalAxisLimits)
*/
void CQwtDataSequenceViewComp::FillChannelData(
			QVector<double>& xData, QVector<double>& yData,
			double& minValue, double& maxValue,
			const int channelIndex, const double& hMinValue, const double& hMaxValue,
			const imeas::IDataSequence* dataSequencePtr) const
{
	if (dataSequencePtr != NULL){
		const int samplesCount = dataSequencePtr->GetSamplesCount();

		imath::CVarVector verticalLinesValues;
		bool isEquidistant = true;
		int verticalCount = 0;

		const double sampleStep = (hMaxValue - hMinValue) / (samplesCount - 1);

		if (m_verticalLinesCompPtr.IsValid()){
			verticalCount = m_verticalLinesCompPtr->GetValues().GetElementsCount();
			verticalLinesValues = m_verticalLinesCompPtr->GetValues();
			isEquidistant = (samplesCount != verticalCount);
		}

		for (int sampleIndex = 0; sampleIndex < samplesCount; sampleIndex++){
			double sample = dataSequencePtr->GetSample(sampleIndex, channelIndex);

			if (isEquidistant){
				xData[sampleIndex] = hMinValue + sampleIndex * sampleStep;
			}
			else{
				xData[sampleIndex] = verticalLinesValues.GetElement(sampleIndex);
			}

			yData[sampleIndex] = sample;

			if (sample > maxValue){
				maxValue = sample;
			}

			if (sample < minValue){
				minValue = sample;
			}
		}

		maxValue = qCeil(maxValue * 100) / 100.0;
	}
}


void CQwtDataSequenceViewComp::SetAxisLimits(const double hMinValue, const double hMaxValue, const double vMinValue, const double vMaxValue)
{
	// HORIzONTAL axis
	double hMin = hMinValue;
	double hMax = hMaxValue;
	if (m_horizontalAxisStartAttrPtr.IsValid()){
		const double hStartValue = *m_horizontalAxisStartAttrPtr;
		hMin = (hStartValue < hMinValue) ? hStartValue : hMinValue;
	}

	if (m_horizontalAxisEndAttrPtr.IsValid()){

		const double userMax = *m_horizontalAxisEndAttrPtr;
		const double delta = qAbs(hMin - hMinValue);

		hMax = (hMaxValue < userMax) ? userMax : hMaxValue + delta;
	}

	/*const int hStep = (int)((hMaxValue - hMinValue) / verticalCount);*/
	m_plotPtr->setAxisScale(QwtPlot::xBottom, hMin, hMax/*, hStep*/);
	m_plotPtr->setAxisMaxMinor(QwtPlot::xBottom, 4);
	
	//VERTICAL axis
	double vMin = vMinValue;
	double vMax = vMaxValue;
	double userMax = 0;
	if (m_verticalAxisEndAttrPtr.IsValid()){
		userMax = *m_verticalAxisEndAttrPtr;
	}
	if (m_verticalAxisStartAttrPtr.IsValid()){
		const double vStartValue = *m_verticalAxisStartAttrPtr;
		vMin = (vMinValue < vStartValue) ? vMinValue : vStartValue;
		const double delta = qAbs(vMin - vMinValue);
		vMax = (vMaxValue < userMax) ? userMax : vMaxValue + delta;
	}

	m_plotPtr->setAxisScale(QwtPlot::yLeft, vMin, vMax, (vMax - vMin) / 10);
	m_plotPtr->setAxisMaxMinor(QwtPlot::yLeft, 5);
}


/*Style of the data plot: 0 No curve; 1 - Lines; 2 - Sticks; 3 - Steps; 4 - Dots*/
QwtPlotCurve::CurveStyle CQwtDataSequenceViewComp::GetQwtPlotStyle() const
{
	if (m_plotStyleAttrPtr.IsValid()){
		const int choice = *m_plotStyleAttrPtr;
		switch(choice){
		case 0:
			return QwtPlotCurve::NoCurve;
		case 1:
			return QwtPlotCurve::Lines;
		case 2:
			return QwtPlotCurve::Sticks;
		case 3:
			return QwtPlotCurve::Steps;
		case 4:
			return QwtPlotCurve::Dots;
		default:
			return QwtPlotCurve::Steps;
		}
	}

	return QwtPlotCurve::Steps;
}


QwtSymbol::Style CQwtDataSequenceViewComp::GetQwtSymbolStyle() const
{
	if (m_symbolStyleAttrPtr.IsValid()){
		const int choice = *m_symbolStyleAttrPtr;

		switch (choice){
		case 0:
			return QwtSymbol::NoSymbol;
		case 1:
			//! Ellipse or circle
			return QwtSymbol::Ellipse;
		case 2:
			//! Rectangle
			return QwtSymbol::Rect;
		case 3:
			//!  Diamond
			return QwtSymbol::Diamond;
		case 4:
			//! Triangle pointing upwards
			return QwtSymbol::Triangle;
		case 5:
			//! Cross (+)
			return QwtSymbol::Cross;
		case 6:
			//! Diagonal cross (X)
			return QwtSymbol::XCross;
		case 7:
			//! Six-pointed star
			return QwtSymbol::Star2;
		default:
			return QwtSymbol::NoSymbol;
		}
	}

	return QwtSymbol::NoSymbol;
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
	imeas::IDataSequence* objectPtr = m_parent.GetObservedObject();
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

	if (currentCurveIndex <= 0){
		double distance = DBL_MAX;
		int channelsCount = objectPtr->GetChannelsCount();
		for (int channelsIndex = 0;channelsIndex < channelsCount; ++channelsIndex){
			double currentSample = objectPtr->GetSample(sampleIndex, channelsIndex);
			double diff = fabs(currentSample-positionY);
			if (diff < distance){
				distance = diff;
				sample = currentSample;
			}
		}
	}
	else{
		sample = objectPtr->GetSample(sampleIndex, currentCurveIndex - 1);
	}

	QString text = QString("%1: %2").arg(sampleIndex).arg(sample);

	return text;
}


} // namespace iqwt


