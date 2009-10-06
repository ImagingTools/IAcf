#ifndef iqtswr_CSwissRangerAcquisitionDataViewComp_included
#define iqtswr_CSwissRangerAcquisitionDataViewComp_included


#include "iswr/ISwissRangerAcquisitionData.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

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

	I_BEGIN_COMPONENT(CSwissRangerAcquisitionDataViewComp)
		I_ASSIGN(m_depthImageGuiCompPtr, "SurfaceView", "", true, "SurfaceView")
		I_ASSIGN(m_depthImageObserverCompPtr, "SurfaceView", "", true, "SurfaceView")
		I_ASSIGN(m_intensityImageGuiCompPtr, "DepthImageView", "", true, "DepthImageView")
		I_ASSIGN(m_intensityImageObserverCompPtr, "DepthImageView", "", true, "DepthImageView")
		I_ASSIGN(m_amplitudeImageGuiCompPtr, "AmplitudeImageView", "", true, "AmplitudeImageView")
		I_ASSIGN(m_amplitudeImageObserverCompPtr, "AmplitudeImageView", "", true, "AmplitudeImageView")
		I_ASSIGN(m_confidenceMapGuiCompPtr, "ConfidenceMapView", "", true, "ConfidenceMapView")
		I_ASSIGN(m_confidenceMapObserverCompPtr, "ConfidenceMapView", "", true, "ConfidenceMapView")
	I_END_COMPONENT

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
	I_REF(iqtgui::IGuiObject, m_depthImageGuiCompPtr);
	I_REF(imod::IObserver, m_depthImageObserverCompPtr);
	
	I_REF(iqtgui::IGuiObject, m_intensityImageGuiCompPtr);
	I_REF(imod::IObserver, m_intensityImageObserverCompPtr);
	
	I_REF(iqtgui::IGuiObject, m_amplitudeImageGuiCompPtr);
	I_REF(imod::IObserver, m_amplitudeImageObserverCompPtr);
	
	I_REF(iqtgui::IGuiObject, m_confidenceMapGuiCompPtr);
	I_REF(imod::IObserver, m_confidenceMapObserverCompPtr);
};


} // namespace iqtswr


#endif // !iqtswr_CSwissRangerAcquisitionDataViewComp_included


