#include "iqtswr/CSwissRangerAcquisitionDataViewComp.h"


#include "istd/TChangeNotifier.h"


namespace iqtswr
{


// reimplemented (imod::IModelEditor)

void CSwissRangerAcquisitionDataViewComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated() && (GetObjectPtr() != NULL));
}


void CSwissRangerAcquisitionDataViewComp::UpdateEditor(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	CoherenceThresholdLabel->setText(QString("%1%").arg(CoherenceThresholdSlider->value()));
	MinimalDistanceCropLabel->setText(QString("%1%").arg(MinimalDistanceCropSlider->value()));
	MaximalDistanceCropLabel->setText(QString("%1%").arg(MaximalDistanceCropSlider->value()));
	ResolutionLabel->setText(QString("%1%").arg(int(1.0 / double(ResolutionSlider->value()) * 100)));

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
	int downsamplingFactor = ResolutionSlider->value();

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
	int maxDistance = objectPtr->GetMaxDistance() * (MaximalDistanceCropSlider->value() / 100.0);
	int minDistance = objectPtr->GetMaxDistance() * (MinimalDistanceCropSlider->value() / 100.0);
	double confidenceThresold = CoherenceThresholdSlider->value() / 100.0 * 255.0;

	istd::CIndex2d newSize(imageWidth / downsamplingFactor, imageHeight / downsamplingFactor);

	std::vector<double> outputData(newSize.GetProductVolume());

	for (int y = 0; y < newSize.GetY(); y++){
		double* outputLinePtr = &outputData[y * newSize.GetX()];
		I_WORD* inputLinePtr = (I_WORD*)depthImage.GetLinePtr(y * downsamplingFactor);
		I_BYTE* confidenceLinePtr = (I_BYTE*)confidenceMap.GetLinePtr(y * downsamplingFactor);

		for (int x = 0; x < newSize.GetX(); x++){
			I_BYTE confidence = *(confidenceLinePtr+ downsamplingFactor * x);

			if (confidence > confidenceThresold){
				double distance = *(inputLinePtr + downsamplingFactor * x);
				if (distance < minDistance){
					distance = minDistance;
				}

				double relativeDistance = 100.0 * distance / maxDistance;
				if (relativeDistance > 100){
					relativeDistance = 100;
				}

				outputLinePtr[x] = 100.0 - relativeDistance;
			}
			else{
				outputLinePtr[x] = 0;
			}	
		}
	}

	istd::CChangeNotifier changePtr(&m_depthImage3d);

	imath::CSampledFunction2d::ArgumentType sizes;
	sizes[0] = newSize.GetX();
	sizes[1] = newSize.GetY();

	m_depthImage3d.CreateFunction(&outputData[0], sizes);
}


// private slots:
	
void CSwissRangerAcquisitionDataViewComp::on_ResolutionSlider_valueChanged(int /*value*/)
{
	UpdateEditor();
}


void CSwissRangerAcquisitionDataViewComp::on_MaximalDistanceCropSlider_valueChanged(int /*value*/)
{
	UpdateEditor();
}


void CSwissRangerAcquisitionDataViewComp::on_MinimalDistanceCropSlider_valueChanged(int /*value*/)
{
	UpdateEditor();
}


void CSwissRangerAcquisitionDataViewComp::on_CoherenceThresholdSlider_valueChanged(int /*value*/)
{
	UpdateEditor();
}


} // namespace iqtswr


