#include "iqtswr/CSwissRangerImageViewComp.h"


#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqtswr
{


// reimplemented (imod::IModelEditor)

void CSwissRangerImageViewComp::UpdateModel() const
{
}


void CSwissRangerImageViewComp::UpdateEditor(int /*updateFlags*/)
{
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CSwissRangerImageViewComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iswr::ISwissRangerImage* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const imod::IModel* modelPtr = dynamic_cast<const imod::IModel*>(&objectPtr->GetDepthImage());
		if (modelPtr != NULL && m_depthImage3dObserverCompPtr.IsValid()){
			(const_cast<imod::IModel*>(modelPtr))->AttachObserver(m_depthImage3dObserverCompPtr.GetPtr());
		}
	}
}


void CSwissRangerImageViewComp::OnGuiModelDetached()
{
	iswr::ISwissRangerImage* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const imod::IModel* modelPtr = dynamic_cast<const imod::IModel*>(&objectPtr->GetDepthImage());
		if (modelPtr != NULL && m_depthImage3dObserverCompPtr.IsValid()){
			(const_cast<imod::IModel*>(modelPtr))->DetachObserver(m_depthImage3dObserverCompPtr.GetPtr());
		}
	}

	BaseClass::OnGuiModelDetached();
}


// reimplemented (iqtgui::CComponentBase)

void CSwissRangerImageViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_depthImage3dGuiCompPtr.IsValid()){
		m_depthImage3dGuiCompPtr->CreateGui(DepthImageFrame);
	}

	if (m_depthImage2dGuiCompPtr.IsValid()){
		m_depthImage2dGuiCompPtr->CreateGui(DepthImage2dFrame);
	}

	if (m_amplitudeImageGuiCompPtr.IsValid()){
		m_amplitudeImageGuiCompPtr->CreateGui(AmplitudeImageFrame);
	}

	if (m_confidenceMapGuiCompPtr.IsValid()){
		m_confidenceMapGuiCompPtr->CreateGui(ConfidenceMapFrame);
	}
}


void CSwissRangerImageViewComp::OnGuiDestroyed()
{
	if (m_depthImage3dGuiCompPtr.IsValid()){
		m_depthImage3dGuiCompPtr->DestroyGui();
	}

	if (m_depthImage2dGuiCompPtr.IsValid()){
		m_depthImage2dGuiCompPtr->DestroyGui();
	}

	if (m_amplitudeImageGuiCompPtr.IsValid()){
		m_amplitudeImageGuiCompPtr->DestroyGui();
	}

	if (m_confidenceMapGuiCompPtr.IsValid()){
		m_confidenceMapGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


} // namespace iqtswr


