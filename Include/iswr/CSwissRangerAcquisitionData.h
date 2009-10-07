#ifndef iswr_CSwissRangerAcquisitionData_included
#define iswr_CSwissRangerAcquisitionData_included


#include "istd/CIndex2d.h"

#include "imod/TModelWrap.h"

#include "iimg/CGeneralBitmap.h"

#include "idev/CDeviceInfo.h"

#include "iswr/ISwissRangerAcquisitionData.h"
#include "iswr/CSwissRangerParams.h"


namespace iswr
{


/**
	Parameters od SwissRanger sensor
*/
class CSwissRangerAcquisitionData: virtual public iswr::ISwissRangerAcquisitionData
{
public:
	CSwissRangerAcquisitionData();

	// reimplemented (iswr::ISwissRangerAcquisitionData)
	virtual bool CreateData(
				int maximalDepth,
				const iimg::IBitmap& depthImage,
				const iimg::IBitmap& confidenceMap,
				const iimg::IBitmap& intensityImage,
				const iimg::IBitmap& amplitudeImage,
				const iswr::ISwissRangerParams* paramsPtr = NULL,
				const idev::IDeviceInfo* deviceInfoPtr = NULL);
	virtual double GetMaxDistance() const;
	virtual const iimg::IBitmap& GetDistanceImage() const;
	virtual const iimg::IBitmap& GetDepthImage() const;
	virtual const iimg::IBitmap& GetConfidenceMap() const;
	virtual const iimg::IBitmap& GetIntensityImage() const;
	virtual const iimg::IBitmap& GetAmplitudeImage() const;
	virtual const iswr::ISwissRangerParams& GetAcquisitionParams() const;
	virtual const idev::IDeviceInfo& GetSensorInfo() const;	

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	mutable imod::TModelWrap<iimg::CGeneralBitmap> m_distanceImage;
	imod::TModelWrap<iimg::CGeneralBitmap> m_depthImage;
	imod::TModelWrap<iimg::CGeneralBitmap> m_confidenceMap;
	imod::TModelWrap<iimg::CGeneralBitmap> m_intensityImage;
	imod::TModelWrap<iimg::CGeneralBitmap> m_amplitudeImage;
	imod::TModelWrap<iswr::CSwissRangerParams> m_acquisitionParams;
	idev::CDeviceInfo m_sensorInfo;
	int m_maxDepth;
};


} // namespace iswr


#endif // !iswr_CSwissRangerAcquisitionData_included


