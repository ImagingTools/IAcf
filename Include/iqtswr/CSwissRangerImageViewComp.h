#ifndef iqtswr_CSwissRangerImageViewComp_included
#define iqtswr_CSwissRangerImageViewComp_included


#include "iswr/ISwissRangerImage.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtswr/Generated/ui_CSwissRangerImageViewComp.h"


namespace iqtswr
{


class CSwissRangerImageViewComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CSwissRangerImageViewComp,
			iswr::ISwissRangerImage>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CSwissRangerImageViewComp,
				iswr::ISwissRangerImage> BaseClass;

	I_BEGIN_COMPONENT(CSwissRangerImageViewComp)
		I_ASSIGN(m_depthImage3dGuiCompPtr, "SurfaceView", "", true, "SurfaceView")
		I_ASSIGN(m_depthImage3dObserverCompPtr, "SurfaceView", "", true, "SurfaceView")
		I_ASSIGN(m_depthImage2dGuiCompPtr, "DepthImage2dView", "", true, "DepthImage2dView")
		I_ASSIGN(m_depthImage2dObserverCompPtr, "DepthImage2dView", "", true, "DepthImage2dView")
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
	I_REF(iqtgui::IGuiObject, m_depthImage3dGuiCompPtr);
	I_REF(imod::IObserver, m_depthImage3dObserverCompPtr);
	
	I_REF(iqtgui::IGuiObject, m_depthImage2dGuiCompPtr);
	I_REF(imod::IObserver, m_depthImage2dObserverCompPtr);
	
	I_REF(iqtgui::IGuiObject, m_amplitudeImageGuiCompPtr);
	I_REF(imod::IObserver, m_amplitudeImageObserverCompPtr);
	
	I_REF(iqtgui::IGuiObject, m_confidenceMapGuiCompPtr);
	I_REF(imod::IObserver, m_confidenceMapObserverCompPtr);
};


} // namespace iqtswr


#endif // !iqtswr_CSwissRangerImageViewComp_included


