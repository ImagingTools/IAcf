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
		I_ASSIGN(m_resultConsumerCompPtr, "ResultConsumer", "Consumer of result messages with geometrical layout", false, "ResultConsumer");
		I_ASSIGN(m_tempConsumerCompPtr, "TempConsumer", "Consumer of temporary result messages with geometrical layout", false, "TempConsumer");
	I_END_COMPONENT;

	enum MessageId
	{
		MI_FOUND_BLOB = 0x9354f0
	};

protected:
	// reimplemented (iblob::CBlobProcessorCompBase)
	virtual bool CalculateBlobs(
				const iprm::IParamsSet* paramsPtr,
				const iblob::IBlobFilterParams* filterParamsPtr,
				const i2d::IObject2d* aoiPtr,
				const iimg::IBitmap& image,
				iipr::IFeaturesConsumer& result);

private:
	I_REF(ilog::IMessageConsumer, m_resultConsumerCompPtr);
	I_REF(ilog::IMessageConsumer, m_tempConsumerCompPtr);
};


} // namespace iocv


