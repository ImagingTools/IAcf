#pragma once


// ACF-Solutions includes
#include <iblob/CBlobProcessorCompBase.h>


namespace iocv
{


class COcvBlobProcessorComp: public iblob::CBlobProcessorCompBase
{
public:
	typedef iblob::CBlobProcessorCompBase BaseClass;

	I_BEGIN_COMPONENT(COcvBlobProcessorComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iblob::CBlobProcessorCompBase)
	virtual bool CalculateBlobs(
				const iprm::IParamsSet* paramsPtr,
				const iblob::IBlobFilterParams* filterParamsPtr,
				const iimg::IBitmap& image,
				iipr::IFeaturesConsumer& result);
};


} // namespace iocv


