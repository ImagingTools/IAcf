#ifndef iswr_ISwissRangerImage_included
#define iswr_ISwissRangerImage_included


#include "imath/TISampledFunction.h"

#include "iimg/IBitmap.h"

#include "idev/IDeviceInfo.h"

#include "iswr/ISwissRangerParams.h"
#include "iswr/ISwissRangerConstrains.h"


namespace iswr
{


/**
	Definition of SwissRanger image.
*/
class ISwissRangerImage: virtual public iser::ISerializable
{
public:
	/**
		Create image.
	*/
	virtual bool CreateImage(
				I_WORD* depthDataPtr, 
				const iimg::IBitmap& amplitudeBitmap,
				const iswr::ISwissRangerParams* paramsPtr = NULL,
				const idev::IDeviceInfo* deviceInfoPtr = NULL) = 0;

	/**
		Get depth image.
	*/
	virtual const iimg::IBitmap& GetDepthImage() const = 0;

	/**
		Set depth image.
	*/
	virtual void SetDepthImage(const iimg::IBitmap& depthImage) = 0;

	/**
		Get confidence map.
	*/
	virtual const iimg::IBitmap& GetConfidenceMap() const = 0;

	/**
		Set confidence map.
	*/
	virtual void SetConfidenceMap(const iimg::IBitmap& confidenceMap) = 0;

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


#endif // !iswr_ISwissRangerImage_included


