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
				double* depthDataPtr, 
				const iimg::IBitmap& amplitudeBitmap,
				const iswr::ISwissRangerParams* paramsPtr,
				const idev::IDeviceInfo* deviceInfoPtr)
{
	m_depthImage.CreateFunction(depthDataPtr, amplitudeBitmap.GetImageSize().GetX(), amplitudeBitmap.GetImageSize().GetY());
	
	bool retVal = m_amplitudeImage.CopyImageFrom(amplitudeBitmap);

	if (paramsPtr != NULL){
		retVal = retVal && iser::CMemoryReadArchive::CloneObjectByArchive(*paramsPtr, m_acquisitionParams);
	}

	if (paramsPtr != NULL){
		retVal = retVal && iser::CMemoryReadArchive::CloneObjectByArchive(*deviceInfoPtr, m_sensorInfo);
	}

	return retVal;
}


const imath::ISampledFunction2d& CSwissRangerImage::GetDepthImage() const
{
	return m_depthImage;
}


const iimg::IBitmap& CSwissRangerImage::GetAmplitudeImage() const
{
	return m_amplitudeImage;
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
//	retVal = retVal && m_depthImage.Serialize(archive);
	retVal = retVal && archive.EndTag(depthImageTag);

	static iser::CArchiveTag amplitudeImageTag("AmplitudeImage", "AmplitudeImage");
	retVal = retVal && archive.BeginTag(amplitudeImageTag);
	retVal = retVal && m_amplitudeImage.Serialize(archive);
	retVal = retVal && archive.EndTag(amplitudeImageTag);

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


