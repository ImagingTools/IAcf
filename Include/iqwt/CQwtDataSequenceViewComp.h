#ifndef iqwt_CQwtDataSequenceViewComp_included
#define iqwt_CQwtDataSequenceViewComp_included


// Qwt includes
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>

// ACF includes
#include <istd/TPointerVector.h>
#include <imod/CModelProxy.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>

// ACF-Solutions includes
#include <imeas/IDataSequence.h>
#include <imeas/IDataSequenceStatistics.h>
#include <imeas/INumericValue.h>

#include <GeneratedFiles/iqwt/ui_CQwtDataSequenceViewComp.h>


class QwtSymbol;

namespace iqwt
{

	

/**
	Component for visualization of histogram data.
*/
class CQwtDataSequenceViewComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CQwtDataSequenceViewComp, imeas::IDataSequence>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CQwtDataSequenceViewComp, imeas::IDataSequence> BaseClass;

	I_BEGIN_COMPONENT(CQwtDataSequenceViewComp);
		I_ASSIGN(m_horizontalLinesCompPtr, "HorizontalLines", "List of y-coordinates of horizontal lines", false, "HorizontalLines")
		I_ASSIGN(m_verticalAxisStartAttrPtr, "VerticalStartValue", "Vertical axis start value", false, 0.0);
		I_ASSIGN(m_verticalLinesCompPtr, "VerticalLines", "List of x-coordinates of vertical lines", false, "VerticalLines");
		I_ASSIGN(m_horizontalAxisStartAttrPtr, "HorizontalStartValue", "Horizontal axis start value", false, 0.0);
		I_ASSIGN(m_plotStyleAttrPtr, "DataPlotStyle", "Style of the data plot\n0 - No curve\n1 - Lines\n2 - Sticks\n3 - Steps\n4 - Dots", true, 3);
		I_ASSIGN(m_symbolStyleAttrPtr, 
			"DataSymbolStyle", "Style of the point symbol \n0 - No symbol\n1 - Ellipse\n2 - Rect\n3 - Diamond\n4 - Triangle\n5 - Cross\n6 - XCross\n7 - Star2",
			true, 0);
	I_END_COMPONENT;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private Q_SLOTS:
	void on_ChannelCombo_currentIndexChanged(int index);

private:
	void ClearPlot();
	void ClearMarkers(istd::TPointerVector<QwtPlotMarker>& markers);

	void MakeHorizontalAxisLimits(double& minValue, double& maxValue) const;
	void MakeValueLines(int& linesCount, const bool isVertical, const imeas::INumericValue* linesCompPtr, istd::TPointerVector<QwtPlotMarker>& markers);
	void FillChannelData(QVector<double>& xData, QVector<double>& yData, double& minValue, double& maxValue, const int channelIndex, const double& hMinValue, const double& hMaxValue, const imeas::IDataSequence* dataSequence) const;
	void SetAxisLimits(const double hMinValue, const double hMaxValue, const double vMinValue, const double vMaxValue);
	QwtPlotCurve::CurveStyle GetQwtPlotStyle() const;
	QwtSymbol::Style GetQwtSymbolStyle() const;

private:
	class DataSequencePlotPicker: public QwtPlotPicker
	{
	public:
		typedef QwtPlotPicker BaseClass;

		DataSequencePlotPicker(CQwtDataSequenceViewComp& parent, int xAxis, int yAxis, QWidget* canvasPtr);

		// reimplemented (QwtPlotPicker)
		virtual QwtText trackerText(const QPoint& position) const;

	private:
		CQwtDataSequenceViewComp& m_parent;
	};

private:
	istd::TDelPtr<QwtPlot> m_plotPtr;
	istd::TPointerVector<QwtPlotCurve> m_channelCurves;
	istd::TPointerVector<QwtPlotMarker> m_verticalMarkers;
	istd::TPointerVector<QwtPlotMarker> m_horizontalMarkers;

	I_REF(imeas::INumericValue, m_horizontalLinesCompPtr);
	I_ATTR(double, m_verticalAxisStartAttrPtr);

	I_REF(imeas::INumericValue, m_verticalLinesCompPtr);
	I_ATTR(double, m_horizontalAxisStartAttrPtr);

	I_ATTR(int, m_plotStyleAttrPtr);
	I_ATTR(int, m_symbolStyleAttrPtr);
};


} // namespace iqwt


#endif // !iqwt_CQwtDataSequenceViewComp_included


