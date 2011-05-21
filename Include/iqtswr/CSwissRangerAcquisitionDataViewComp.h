#ifndef iqtswr_CSwissRangerAcquisitionDataViewComp_included
#define iqtswr_CSwissRangerAcquisitionDataViewComp_included


// ACF includes
#include "imath/CSampledFunction2d.h"
#include "imod/TModelWrap.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"


// IACF includes
#include "iswr/ISwissRangerAcquisitionData.h"

#include "iqtswr/Generated/ui_CSwissRangerAcquisitionDataViewComp.h"


namespace iqtswr
{


class CSwissRangerAcquisitionDataViewComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CSwissRangerAcquisitionDataViewComp,
			iswr::ISwissRangerAcquisitionData>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CSwissRangerAcquisitionDataViewComp,
				iswr::ISwissRangerAcquisitionData> BaseClass;

	I_BEGIN_COMPONENT(CSwissRangerAcquisitionDataViewComp);
		I_ASSIGN(m_depthImageGuiCompPtr, "DepthImageView", "", true, "DepthImageView");
		I_ASSIGN_TO(m_depthImageObserverCompPtr, m_depthImageGuiCompPtr, true);
		I_ASSIGN(m_depthImageGui3dCompPtr, "DepthImageView3D", "", true, "DepthImageView3D");
		I_ASSIGN_TO(m_depthImageObserver3dCompPtr, m_depthImageGui3dCompPtr, true);
		I_ASSIGN(m_amplitudeImageGuiCompPtr, "AmplitudeImageView", "", true, "AmplitudeImageView");
		I_ASSIGN_TO(m_amplitudeImageObserverCompPtr, m_amplitudeImageGuiCompPtr, true);
		I_ASSIGN(m_confidenceMapGuiCompPtr, "ConfidenceMapView", "", true, "ConfidenceMapView");
		I_ASSIGN_TO(m_confidenceMapObserverCompPtr, m_confidenceMapGuiCompPtr, true);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

	// reimplemented (iqtgui::CComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	void Create3dModel();

private Q_SLOTS:
	void on_ResolutionSlider_valueChanged(int value);
	void on_MaximalDistanceCropSlider_valueChanged(int value);
	void on_MinimalDistanceCropSlider_valueChanged(int value);
	void on_CoherenceThresholdSlider_valueChanged(int value);

private:
	I_REF(iqtgui::IGuiObject, m_depthImageGuiCompPtr);
	I_REF(imod::IObserver, m_depthImageObserverCompPtr);
	
	I_REF(iqtgui::IGuiObject, m_depthImageGui3dCompPtr);
	I_REF(imod::IObserver, m_depthImageObserver3dCompPtr);
	
	I_REF(iqtgui::IGuiObject, m_amplitudeImageGuiCompPtr);
	I_REF(imod::IObserver, m_amplitudeImageObserverCompPtr);
	
	I_REF(iqtgui::IGuiObject, m_confidenceMapGuiCompPtr);
	I_REF(imod::IObserver, m_confidenceMapObserverCompPtr);

	imod::TModelWrap<imath::CSampledFunction2d> m_depthImage3d;
};


} // namespace iqtswr


#endif // !iqtswr_CSwissRangerAcquisitionDataViewComp_included


