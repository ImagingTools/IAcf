#pragma once


// ACF includes
#include "iproc/TSyncProcessorCompBase.h"

// ACF-Solutions includes
#include "iipr/IFeaturesConsumer.h"
#include "iipr/IImageToFeatureProcessor.h"
#include "iblob/IBlobFilterParams.h"


namespace iocv
{


class COcvBlobProcessorComp:
			public iproc::TSyncProcessorCompBase<iipr::IImageToFeatureProcessor>
{
public:
	typedef iproc::TSyncProcessorCompBase<iipr::IImageToFeatureProcessor> BaseClass;

	I_BEGIN_COMPONENT(COcvBlobProcessorComp);
		I_ASSIGN(m_filterParamsIdAttrPtr, "FilterParamsId", "ID of blob filter parameters in the parameter set", true, "FilterParams");
	I_END_COMPONENT;

	// reimplemented (iipr::IImageToFeatureProcessor)
	virtual int DoExtractFeatures(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap& image,
				iipr::IFeaturesConsumer& results);

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);
protected:
	bool CalculateBlobs(
				const iimg::IBitmap& bitmap,
				const iblob::IBlobFilterParams* filterParamsPtr,
				iipr::IFeaturesConsumer& result);

private:
	I_ATTR(QByteArray, m_filterParamsIdAttrPtr);
};


} // namespace iocv


