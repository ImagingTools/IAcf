#ifndef iswr_ISwissRangerAcquisitionData_included
#define iswr_ISwissRangerAcquisitionData_included


#include "iimg/IBitmap.h"


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
				int maximalDepth,
				const iimg::IBitmap& depthImage,
				const iimg::IBitmap& confidenceMap,
				const iimg::IBitmap& intensityImage,
				const iimg::IBitmap& amplitudeImage,
				const I_SWORD* xBufferPtr,
				const I_SWORD* yBufferPtr) = 0;

	virtual double GetMaxDistance() const = 0;

	/**
		Get depth image.
	*/
	virtual const iimg::IBitmap& GetDepthImage() const = 0;

	/**
		Get 8-bit distance image.
	*/
	virtual const iimg::IBitmap& GetDistanceImage() const = 0;

	/**
		Get confidence map.
	*/
	virtual const iimg::IBitmap& GetConfidenceMap() const = 0;

	/**
		Get intensity image.
	*/
	virtual const iimg::IBitmap& GetIntensityImage() const = 0;

	/**
		Get amplitude image
	*/
	virtual const iimg::IBitmap& GetAmplitudeImage() const = 0;

	/**
		Get X-coordinates buffer. Coordinates are in mm.
	*/
	virtual const I_SWORD* GetXCoordinatesBuffer() const = 0;

	/**
		Get Y-coordinates buffer. Coordinates are in mm.
	*/
	virtual const I_SWORD* GetYCoordinatesBuffer() const = 0;
};


} // namespace iswr


#endif // !iswr_ISwissRangerAcquisitionData_included


