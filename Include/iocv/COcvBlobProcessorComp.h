#pragma once


// STD
#include <vector>

// OpenCV includes
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>

// ACF-Solutions includes
#include <iblob/CBlobProcessorCompBase.h>
#include <iprm/IEnableableParam.h>


namespace iocv
{


class COcvBlobProcessorComp: public iblob::CBlobProcessorCompBase
{
public:
	typedef iblob::CBlobProcessorCompBase BaseClass;

	I_BEGIN_COMPONENT(COcvBlobProcessorComp);
		I_ASSIGN(m_resultConsumerCompPtr, "ResultConsumer", "Consumer of result messages with geometrical layout", false, "ResultConsumer");
		I_ASSIGN(m_tempConsumerCompPtr, "TempConsumer", "Consumer of temporary result messages with geometrical layout", false, "TempConsumer");
		I_ASSIGN(m_getNegativeBlobsPolygonCompPtr, "GetNegativeBlobsPolygon", "If undefined then returns all blobs, if true then returns negative polarity blobs else returns positive polarity blobs", false, "GetNegativeBlobsPolygon");
		I_ASSIGN(m_calibrationProviderCompPtr, "CalibrationProvider", "Provides calibration for output blob polygons", false, "CalibrationProvider");
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
	I_REF(iprm::IEnableableParam, m_getNegativeBlobsPolygonCompPtr);
	I_REF(i2d::ICalibrationProvider, m_calibrationProviderCompPtr);
};


} // namespace iocv


