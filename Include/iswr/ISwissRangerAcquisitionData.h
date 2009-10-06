#ifndef iswr_ISwissRangerAcquisitionData_included
#define iswr_ISwissRangerAcquisitionData_included


#include "imath/TISampledFunction.h"

#include "iimg/IBitmap.h"

#include "idev/IDeviceInfo.h"

#include "iswr/ISwissRangerParams.h"
#include "iswr/ISwissRangerConstrains.h"


namespace iswr
{


/**
	Definition of SwissRanger camera output.
*/
class ISwissRangerAcquisitionData: virtual public iser::ISerializable
{
public:
	/**
		Create image.
	*/
	virtual bool CreateData(
				I_WORD* depthDataPtr,
				int maximalDepth,
				const istd::CIndex2d& imageSize,
				const iimg::IBitmap& confidenceMap,
				const iimg::IBitmap& intensityImage,
				const iimg::IBitmap& amplitudeImage,
				const iswr::ISwissRangerParams* paramsPtr = NULL,
				const idev::IDeviceInfo* deviceInfoPtr = NULL,
				bool releaseFlag = true) = 0;

	/**
		Get raw depth data.
	*/
	virtual const I_WORD* GetDepthData() const = 0;

	/**
		Get depth image.
	*/
	virtual const iimg::IBitmap& GetDepthImage() const = 0;

	/**
		Get confidence map.
	*/
	virtual const iimg::IBitmap& GetConfidenceMap() const = 0;

	virtual const iimg::IBitmap& GetIntensityImage() const = 0;

	virtual const iimg::IBitmap& GetAmplitudeImage() const = 0;

	/**
		Get the acquisition parameter for this image.
	*/
	virtual const iswr::ISwissRangerParams& GetAcquisitionParams() const = 0;

	/**
		Get the sensor info for this image.
	*/
	virtual const idev::IDeviceInfo& GetSensorInfo() const = 0;
};


} // namespace iswr


#endif // !iswr_ISwissRangerAcquisitionData_included


