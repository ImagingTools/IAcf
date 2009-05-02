#ifndef iswr_CSwissRangerImage_included
#define iswr_CSwissRangerImage_included


#include "imath/CSampledFunction2d.h"

#include "imod/TModelWrap.h"

#include "iimg/CGeneralBitmap.h"

#include "idev/CDeviceInfo.h"

#include "iswr/ISwissRangerImage.h"
#include "iswr/CSwissRangerParams.h"


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
	virtual bool CreateImage(
				double* depthDataPtr, 
				const iimg::IBitmap& amplitudeBitmap,
				const iswr::ISwissRangerParams* paramsPtr = NULL,
				const idev::IDeviceInfo* deviceInfoPtr = NULL);

	virtual const imath::ISampledFunction2d& GetDepthImage() const;
	virtual const iimg::IBitmap& GetAmplitudeImage() const;
	virtual const iswr::ISwissRangerParams& GetAcquisitionParams() const;
	virtual const idev::IDeviceInfo& GetSensorInfo() const;	

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	imod::TModelWrap<imath::CSampledFunction2d> m_depthImage;
	imod::TModelWrap<iimg::CGeneralBitmap> m_amplitudeImage;
	imod::TModelWrap<iswr::CSwissRangerParams> m_acquisitionParams;
	idev::CDeviceInfo m_sensorInfo;
};


} // namespace iswr


#endif // !iswr_CSwissRangerImage_included


