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
	Create3dModel();
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CSwissRangerAcquisitionDataViewComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iswr::ISwissRangerAcquisitionData* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const imod::IModel* modelPtr = dynamic_cast<const imod::IModel*>(&objectPtr->GetDistanceImage());
		if (modelPtr != NULL && m_depthImageObserverCompPtr.IsValid()){
			(const_cast<imod::IModel*>(modelPtr))->AttachObserver(m_depthImageObserverCompPtr.GetPtr());
		}
		modelPtr = dynamic_cast<const imod::IModel*>(&objectPtr->GetConfidenceMap());
		if (modelPtr != NULL && this->m_confidenceMapObserverCompPtr.IsValid()){
			(const_cast<imod::IModel*>(modelPtr))->AttachObserver(m_confidenceMapObserverCompPtr.GetPtr());
		}
		modelPtr = dynamic_cast<const imod::IModel*>(&objectPtr->GetAmplitudeImage());
		if (modelPtr != NULL && this->m_amplitudeImageGuiCompPtr.IsValid()){
			(const_cast<imod::IModel*>(modelPtr))->AttachObserver(m_amplitudeImageObserverCompPtr.GetPtr());
		}

		if (m_depthImageObserver3dCompPtr.IsValid()){
			m_depthImage3d.AttachObserver(m_depthImageObserver3dCompPtr.GetPtr());
		}
	}
}


void CSwissRangerAcquisitionDataViewComp::OnGuiModelDetached()
{
	iswr::ISwissRangerAcquisitionData* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const imod::IModel* modelPtr = dynamic_cast<const imod::IModel*>(&objectPtr->GetDistanceImage());
		if (modelPtr != NULL && m_depthImageObserverCompPtr.IsValid()){
			(const_cast<imod::IModel*>(modelPtr))->DetachObserver(m_depthImageObserverCompPtr.GetPtr());
		}
		modelPtr = dynamic_cast<const imod::IModel*>(&objectPtr->GetConfidenceMap());
		if (modelPtr != NULL && this->m_confidenceMapObserverCompPtr.IsValid()){
			(const_cast<imod::IModel*>(modelPtr))->DetachObserver(m_confidenceMapObserverCompPtr.GetPtr());
		}
		modelPtr = dynamic_cast<const imod::IModel*>(&objectPtr->GetAmplitudeImage());
		if (modelPtr != NULL && this->m_amplitudeImageGuiCompPtr.IsValid()){
			(const_cast<imod::IModel*>(modelPtr))->DetachObserver(m_amplitudeImageObserverCompPtr.GetPtr());
		}	

		if (m_depthImageObserver3dCompPtr.IsValid()){
			m_depthImage3d.DetachObserver(m_depthImageObserver3dCompPtr.GetPtr());
		}
	}

	BaseClass::OnGuiModelDetached();
}


// reimplemented (iqtgui::CComponentBase)

void CSwissRangerAcquisitionDataViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_depthImageGuiCompPtr.IsValid()){
		m_depthImageGuiCompPtr->CreateGui(DepthImageFrame2d);
	}

	if (m_depthImageGui3dCompPtr.IsValid()){
		m_depthImageGui3dCompPtr->CreateGui(DepthImageFrame3d);
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

	if (m_depthImageGui3dCompPtr.IsValid()){
		m_depthImageGui3dCompPtr->DestroyGui();
	}

	if (m_amplitudeImageGuiCompPtr.IsValid()){
		m_amplitudeImageGuiCompPtr->DestroyGui();
	}

	if (m_confidenceMapGuiCompPtr.IsValid()){
		m_confidenceMapGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


void CSwissRangerAcquisitionDataViewComp::Create3dModel()
{
	int downsamplingFactor = 2;

	iswr::ISwissRangerAcquisitionData* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}
	
	const iimg::IBitmap& depthImage = objectPtr->GetDepthImage();
	if (depthImage.IsEmpty()){
		return;
	}

	const iimg::IBitmap& confidenceMap = objectPtr->GetConfidenceMap();
	if (confidenceMap.IsEmpty()){
		return;
	}

	int imageWidth = depthImage.GetImageSize().GetX();
	int imageHeight = depthImage.GetImageSize().GetY();
	int maxDistance = objectPtr->GetMaxDistance();

	istd::CIndex2d newSize(imageWidth / downsamplingFactor, imageHeight / downsamplingFactor);

	istd::TDelPtr<double, true> outputDataPtr(new double[newSize.GetProductVolume()]);

	for (int y = 0; y < newSize.GetY(); y++){
		double* outputLinePtr = outputDataPtr.GetPtr() + y * newSize.GetX();
		I_WORD* inputLinePtr = (I_WORD*)depthImage.GetLinePtr(y * downsamplingFactor);
		//I_BYTE* confidenceLinePtr = (I_BYTE*)confidenceMap.GetLinePtr(y * downsamplingFactor);

		for (int x = 0; x < newSize.GetX(); x++){
//			I_BYTE confidence = *(confidenceLinePtr+ downsamplingFactor * x);

			outputLinePtr[x] = 100  - 100 * *(inputLinePtr + downsamplingFactor * x) / maxDistance;
		}
	}

	istd::CChangeNotifier changePtr(&m_depthImage3d);

	imath::CSampledFunction2d::ArgumentType sizes;
	sizes[0] = newSize.GetX();
	sizes[1] = newSize.GetY();

	m_depthImage3d.CreateFunction(outputDataPtr.GetPtr(), sizes);
}


} // namespace iqtswr


