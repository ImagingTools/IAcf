// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-IAcf-Commercial
#ifndef iqwt_CHistogramViewComp_included
#define iqwt_CHistogramViewComp_included


// Qwt includes
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_picker.h>

// ACF includes
#include <istd/TPointerVector.h>
#include <imod/CModelProxy.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>

// ACF-Solutions includes
#include <imeas/IDataSequence.h>
#include <imeas/IDataSequenceStatistics.h>

#include <GeneratedFiles/iqwt/ui_CHistogramViewComp.h>


namespace iqwt
{


/**
	Component for visualization of histogram data.
*/
class CHistogramViewComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CHistogramViewComp, imeas::IDataSequence>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CHistogramViewComp, imeas::IDataSequence> BaseClass;

	I_BEGIN_COMPONENT(CHistogramViewComp);
		I_ASSIGN(m_dataStatisticsGuiCompPtr, "StatisticsGui", "GUI for the data statistics", false, "StatisticsGui");
		I_ASSIGN_TO(m_dataStatisticsObserverCompPtr, m_dataStatisticsGuiCompPtr, false);
		I_ASSIGN(m_dataSequenceStatisticsCompPtr, "DataSequenceStatistics", "Data sequence statistics", false, "DataSequenceStatistics");
	I_END_COMPONENT;

	// reimplemenented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiDestroyed() override;

private Q_SLOTS:
	void on_ChannelCombo_currentIndexChanged(int index);

private:
	void ClearPlot();

private:
	class HistogramPlotPicker: public QwtPlotPicker
	{
	public:
		typedef QwtPlotPicker BaseClass;

		HistogramPlotPicker(CHistogramViewComp& parent, int xAxis, int yAxis, QWidget* canvasPtr);

		// reimplemented (QwtPlotPicker)
		virtual QwtText trackerText(const QPoint& position) const;

	private:
		CHistogramViewComp& m_parent;
	};

private:
	istd::TDelPtr<QwtPlot> m_plotPtr;
	istd::TPointerVector<QwtPlotCurve> m_channelCurves;
	
	imod::CModelProxy m_curentChannelStatisticsModel;

	I_REF(iqtgui::IGuiObject, m_dataStatisticsGuiCompPtr);
	I_REF(imod::IObserver, m_dataStatisticsObserverCompPtr);
	I_REF(imeas::IDataSequenceStatistics, m_dataSequenceStatisticsCompPtr);
};


} // namespace iqwt


#endif // !iqwt_CHistogramViewComp_included


