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
		Get depth image.
	*/
	virtual const imath::ISampledFunction2d& GetDepthImage() const = 0;

	/**
		Get amplitude image.
	*/
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


#endif // !iswr_ISwissRangerImage_included


