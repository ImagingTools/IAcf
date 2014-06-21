#ifndef iqwt_CQwtDataSequenceViewComp_included
#define iqwt_CQwtDataSequenceViewComp_included


// Qwt includes
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_picker.h"
#include "qwt_plot_marker.h"


// ACF includes
#include "istd/TPointerVector.h"

#include "imod/CModelProxy.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "imeas/IDataSequence.h"
#include "imeas/IDataSequenceStatistics.h"
#include "imeas/INumericValue.h"

#include "GeneratedFiles/iqwt/ui_CQwtDataSequenceViewComp.h"


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
		I_ASSIGN_MULTI_0(m_horizontalLinesCompPtr, "HorizontalLines", "List of y-coordinates of horizontal lines", false);
		I_ASSIGN_MULTI_0(m_verticalLinesCompPtr, "VerticalLines", "List of x-coordinates of vertical lines", false);
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
	void ClearMarkers();

private:
	class DataSequencePlotPicker: public QwtPlotPicker
	{
	public:
		typedef QwtPlotPicker BaseClass;

		DataSequencePlotPicker(CQwtDataSequenceViewComp& parent, int xAxis, int yAxis, QwtPlotCanvas* canvasPtr);

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

	I_MULTIREF(imeas::INumericValue, m_verticalLinesCompPtr);
	I_MULTIREF(imeas::INumericValue, m_horizontalLinesCompPtr);
};


} // namespace iqwt


#endif // !iqwt_CQwtDataSequenceViewComp_included


