#include "iipr/CFeatureToImageMapperProcessorComp.h"


namespace iipr
{


// reimplemented (iipr::IFeatureToFeatureProcessor)

int CFeatureToImageMapperProcessorComp::DoConvertFeatures(
			const iprm::IParamsSet* paramsPtr,
			const IFeaturesContainer& container,
			IFeaturesConsumer& results)
{
	if (!m_featuresMapperCompPtr.IsValid()){
		return TS_INVALID;
	}

	IFeaturesContainer::Features features = container.GetFeatures();

	for (		IFeaturesContainer::Features::const_iterator iter = features.begin();
				iter != features.end();
				++iter){
		const IFeature* featurePtr = *iter;
		I_ASSERT(featurePtr != NULL);

		i2d::CVector2d position;
		if (m_featuresMapperCompPtr->GetImagePosition(*featurePtr, paramsPtr, position)){
			PositionFeature* positionFeaturePtr = new PositionFeature(featurePtr->GetWeight());
			positionFeaturePtr->SetPosition(position);

			bool isFull = false;
			results.AddFeature(positionFeaturePtr, &isFull);

			if (isFull){
				break;
			}
		}
		else if (!*m_ignoreNotMappableAttrPtr){
			return TS_INVALID;
		}
	}

	return TS_OK;
}


// reimplemented (iproc::IProcessor)

int CFeatureToImageMapperProcessorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	const IFeaturesContainer* containerPtr = dynamic_cast<const IFeaturesContainer*>(inputPtr);
	IFeaturesConsumer* consumerPtr = dynamic_cast<IFeaturesConsumer*>(outputPtr);

	if (		(containerPtr == NULL) ||
				(consumerPtr == NULL)){
		return TS_INVALID;
	}

	return DoConvertFeatures(paramsPtr, *containerPtr, *consumerPtr);
}


} // namespace iipr


