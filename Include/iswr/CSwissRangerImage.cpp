#include "iswr/CSwissRangerImage.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "iser/CMemoryReadArchive.h"


namespace iswr
{


// public methods

CSwissRangerImage::CSwissRangerImage()
{
}


// reimplemented (iswr::ISwissRangerImage)

bool CSwissRangerImage::CreateImage(
				I_WORD* depthDataPtr, 
				const iimg::IBitmap& confidenceImage,
				const iswr::ISwissRangerParams* paramsPtr,
				const idev::IDeviceInfo* deviceInfoPtr)
{
	if (!m_depthImage.CreateBitmap(confidenceImage.GetImageSize(), depthDataPtr, true, 0, 16, 1)){
		return false;
	}

	bool retVal = m_confidenceMap.CopyImageFrom(confidenceImage);

	if (paramsPtr != NULL){
		retVal = retVal && iser::CMemoryReadArchive::CloneObjectByArchive(*paramsPtr, m_acquisitionParams);
	}

	if (paramsPtr != NULL){
		retVal = retVal && iser::CMemoryReadArchive::CloneObjectByArchive(*deviceInfoPtr, m_sensorInfo);
	}

	return retVal;
}


const iimg::IBitmap& CSwissRangerImage::GetDepthImage() const
{
	return m_depthImage;
}


void CSwissRangerImage::SetDepthImage(const iimg::IBitmap& depthImage)
{
	m_depthImage.CopyImageFrom(depthImage);
}



const iimg::IBitmap& CSwissRangerImage::GetConfidenceMap() const
{
	return m_confidenceMap;
}


void CSwissRangerImage::SetConfidenceMap(const iimg::IBitmap& confidenceMap)
{
	m_confidenceMap.CopyImageFrom(confidenceMap);
}


const iswr::ISwissRangerParams& CSwissRangerImage::GetAcquisitionParams() const
{
	return m_acquisitionParams;
}


const idev::IDeviceInfo& CSwissRangerImage::GetSensorInfo() const
{
	return m_sensorInfo;
}


// reimplemented (iser::ISerializable)


bool CSwissRangerImage::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag depthImageTag("DepthImage", "DepthImage");
	retVal = retVal && archive.BeginTag(depthImageTag);
	retVal = retVal && m_depthImage.Serialize(archive);
	retVal = retVal && archive.EndTag(depthImageTag);

	static iser::CArchiveTag confidenceMapTag("ConfidenceMap", "ConfidenceMap");
	retVal = retVal && archive.BeginTag(confidenceMapTag);
	retVal = retVal && m_confidenceMap.Serialize(archive);
	retVal = retVal && archive.EndTag(confidenceMapTag);

	static iser::CArchiveTag acquisitionParamsTag("ModulationFrequencyMode", "ModulationFrequencyMode");
	retVal = retVal && archive.BeginTag(acquisitionParamsTag);
	retVal = retVal && m_acquisitionParams.Serialize(archive);
	retVal = retVal && archive.EndTag(acquisitionParamsTag);

	static iser::CArchiveTag sensorInfoTag("SensorInfo", "SensorInfo");
	retVal = retVal && archive.BeginTag(sensorInfoTag);
	retVal = retVal && m_sensorInfo.Serialize(archive);
	retVal = retVal && archive.EndTag(sensorInfoTag);

	return retVal;
}


} // namespace iswr


