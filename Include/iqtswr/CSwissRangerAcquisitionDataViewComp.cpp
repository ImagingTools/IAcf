#include "iqtswr/CSwissRangerAcquisitionDataViewComp.h"


#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqtswr
{


// reimplemented (imod::IModelEditor)

void CSwissRangerAcquisitionDataViewComp::UpdateModel() const
{
}


void CSwissRangerAcquisitionDataViewComp::UpdateEditor(int /*updateFlags*/)
{
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CSwissRangerAcquisitionDataViewComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iswr::ISwissRangerAcquisitionData* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const imod::IModel* modelPtr = dynamic_cast<const imod::IModel*>(&objectPtr->GetDepthImage());
		if (modelPtr != NULL && m_depthImageObserverCompPtr.IsValid()){
			(const_cast<imod::IModel*>(modelPtr))->AttachObserver(m_depthImageObserverCompPtr.GetPtr());
		}
	}
}


void CSwissRangerAcquisitionDataViewComp::OnGuiModelDetached()
{
	iswr::ISwissRangerAcquisitionData* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const imod::IModel* modelPtr = dynamic_cast<const imod::IModel*>(&objectPtr->GetDepthImage());
		if (modelPtr != NULL && m_depthImageObserverCompPtr.IsValid()){
			(const_cast<imod::IModel*>(modelPtr))->DetachObserver(m_depthImageObserverCompPtr.GetPtr());
		}
	}

	BaseClass::OnGuiModelDetached();
}


// reimplemented (iqtgui::CComponentBase)

void CSwissRangerAcquisitionDataViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_depthImageGuiCompPtr.IsValid()){
		m_depthImageGuiCompPtr->CreateGui(DepthImageFrame);
	}

	if (m_intensityImageGuiCompPtr.IsValid()){
		m_intensityImageGuiCompPtr->CreateGui(IntensityImageFrame);
	}

	if (m_amplitudeImageGuiCompPtr.IsValid()){
		m_amplitudeImageGuiCompPtr->CreateGui(AmplitudeImageFrame);
	}

	if (m_confidenceMapGuiCompPtr.IsValid()){
		m_confidenceMapGuiCompPtr->CreateGui(ConfidenceMapFrame);
	}
}


void CSwissRangerAcquisitionDataViewComp::OnGuiDestroyed()
{
	if (m_depthImageGuiCompPtr.IsValid()){
		m_depthImageGuiCompPtr->DestroyGui();
	}

	if (m_intensityImageGuiCompPtr.IsValid()){
		m_intensityImageGuiCompPtr->DestroyGui();
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


