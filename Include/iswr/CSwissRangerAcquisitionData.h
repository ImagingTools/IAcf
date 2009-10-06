#ifndef iswr_CSwissRangerAcquisitionData_included
#define iswr_CSwissRangerAcquisitionData_included


#include "imath/CSampledFunction2d.h"

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
				I_WORD* depthDataPtr,
				int maximalDepth,
				const istd::CIndex2d& imageSize,
				const iimg::IBitmap& confidenceMap,
				const iimg::IBitmap& intensityImage,
				const iimg::IBitmap& amplitudeImage,
				const iswr::ISwissRangerParams* paramsPtr = NULL,
				const idev::IDeviceInfo* deviceInfoPtr = NULL,
				bool releaseFlag = true);

	virtual const I_WORD* GetDepthData() const;

	virtual const iimg::IBitmap& GetDepthImage() const;
	virtual const iimg::IBitmap& GetConfidenceMap() const;
	virtual const iimg::IBitmap& GetIntensityImage() const;
	virtual const iimg::IBitmap& GetAmplitudeImage() const;
	virtual const iswr::ISwissRangerParams& GetAcquisitionParams() const;
	virtual const idev::IDeviceInfo& GetSensorInfo() const;	

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	imod::TModelWrap<iimg::CGeneralBitmap> m_depthImage;
	imod::TModelWrap<iimg::CGeneralBitmap> m_confidenceMap;
	imod::TModelWrap<iimg::CGeneralBitmap> m_intensityImage;
	imod::TModelWrap<iimg::CGeneralBitmap> m_amplitudeImage;
	imod::TModelWrap<iswr::CSwissRangerParams> m_acquisitionParams;
	idev::CDeviceInfo m_sensorInfo;

	istd::TOptDelPtr<I_WORD, true> m_depthDataPtr;
	int m_maxDepth;
};


} // namespace iswr


#endif // !iswr_CSwissRangerAcquisitionData_included


