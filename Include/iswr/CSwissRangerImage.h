#ifndef iswr_CSwissRangerImage_included
#define iswr_CSwissRangerImage_included


#include "iswr/ISwissRangerImage.h"

#include "imath/CSampledFunction2d.h"
#include "iimg/CGeneralBitmap.h"
#include "iswr/CSwissRangerParams.h"
#include "idev/CDeviceInfo.h"


namespace iswr
{


/**
	Parameters od SwissRanger sensor
*/
class CSwissRangerImage: virtual public iswr::ISwissRangerImage
{
public:
	CSwissRangerImage();

	// reimplemented (iswr::ISwissRangerImage)
	virtual const imath::ISampledFunction2d& GetDepthImage() const;
	virtual const iimg::IBitmap& GetAmplitudeImage() const;
	virtual const iswr::ISwissRangerParams& GetAcquisitionParams() const;
	virtual const idev::IDeviceInfo& GetSensorInfo() const;	

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	imath::CSampledFunction2d m_depthImage;
	iimg::CGeneralBitmap m_amplitudeImage;
	iswr::CSwissRangerParams m_acquisitionParams;
	idev::CDeviceInfo m_sensorInfo;
};


} // namespace iswr


#endif // !iswr_CSwissRangerImage_included


