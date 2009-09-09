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
				I_WORD* depthDataPtr, 
				const iimg::IBitmap& amplitudeBitmap,
				const iswr::ISwissRangerParams* paramsPtr = NULL,
				const idev::IDeviceInfo* deviceInfoPtr = NULL);

	virtual const iimg::IBitmap& GetDepthImage() const;
	virtual void SetDepthImage(const iimg::IBitmap& depthImage);
	virtual const iimg::IBitmap& GetConfidenceMap() const;
	virtual void SetConfidenceMap(const iimg::IBitmap& confidenceMap);
	virtual const iswr::ISwissRangerParams& GetAcquisitionParams() const;
	virtual const idev::IDeviceInfo& GetSensorInfo() const;	

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	imod::TModelWrap<iimg::CGeneralBitmap> m_depthImage;
	imod::TModelWrap<iimg::CGeneralBitmap> m_confidenceMap;
	imod::TModelWrap<iswr::CSwissRangerParams> m_acquisitionParams;
	idev::CDeviceInfo m_sensorInfo;
};


} // namespace iswr


#endif // !iswr_CSwissRangerImage_included


