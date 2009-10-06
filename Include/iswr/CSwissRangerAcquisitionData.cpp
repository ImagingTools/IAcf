#include "iswr/CSwissRangerAcquisitionData.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/TDelPtr.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "iser/CMemoryReadArchive.h"


namespace iswr
{


// public methods

CSwissRangerAcquisitionData::CSwissRangerAcquisitionData()
	:m_maxDepth(0)
{
}


// reimplemented (iswr::ISwissRangerAcquisitionData)

bool CSwissRangerAcquisitionData::CreateData(
			I_WORD* depthDataPtr,
			int maximalDepth,
			const istd::CIndex2d& imageSize,
			const iimg::IBitmap& confidenceMap,
			const iimg::IBitmap& intensityImage,
			const iimg::IBitmap& amplitudeImage,
			const iswr::ISwissRangerParams* paramsPtr,
			const idev::IDeviceInfo* deviceInfoPtr,
			bool releaseFlag)
{
	m_maxDepth = maximalDepth;
	int imageWidth = imageSize.GetX();
	int imageHeight = imageSize.GetY();

	istd::TDelPtr<I_BYTE, true> imageDataPtr(new I_BYTE[imageWidth * imageHeight]);

	// convert input data to 8-bit image:
	for (int y = 0; y < imageHeight; y++){
		I_BYTE* imageLinePtr = imageDataPtr.GetPtr() + y * imageWidth;
		I_WORD* inputDataPtr = depthDataPtr + y * imageWidth;
		for (int x = 0; x < imageWidth; x++){
			imageLinePtr[x] = I_BYTE((inputDataPtr[x] / (double)m_maxDepth) * 255);
		}
	}

	if (m_depthImage.CreateBitmap(istd::CIndex2d(imageWidth, imageHeight), imageDataPtr.GetPtr(), true)){
		imageDataPtr.PopPtr();
	
		m_depthDataPtr.SetPtr(depthDataPtr, releaseFlag);
	}
	
	m_confidenceMap.CopyImageFrom(confidenceMap);
	m_intensityImage.CopyImageFrom(intensityImage);
	m_amplitudeImage.CopyImageFrom(amplitudeImage);

	if (paramsPtr != NULL){
		iser::CMemoryReadArchive::CloneObjectByArchive(*paramsPtr, m_acquisitionParams);
	}

	if (paramsPtr != NULL){
		iser::CMemoryReadArchive::CloneObjectByArchive(*deviceInfoPtr, m_sensorInfo);
	}

	return true;
}


const I_WORD* CSwissRangerAcquisitionData::GetDepthData() const
{
	return m_depthDataPtr.GetPtr();
}


const iimg::IBitmap& CSwissRangerAcquisitionData::GetDepthImage() const
{
	return m_depthImage;
}


const iimg::IBitmap& CSwissRangerAcquisitionData::GetConfidenceMap() const
{
	return m_confidenceMap;
}


const iimg::IBitmap& CSwissRangerAcquisitionData::GetIntensityImage() const
{
	return m_intensityImage; 
}


const iimg::IBitmap& CSwissRangerAcquisitionData::GetAmplitudeImage() const
{
	return m_amplitudeImage;
}


const iswr::ISwissRangerParams& CSwissRangerAcquisitionData::GetAcquisitionParams() const
{
	return m_acquisitionParams;
}


const idev::IDeviceInfo& CSwissRangerAcquisitionData::GetSensorInfo() const
{
	return m_sensorInfo;
}


// reimplemented (iser::ISerializable)


bool CSwissRangerAcquisitionData::Serialize(iser::IArchive& archive)
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

	static iser::CArchiveTag intensityImageTag("IntensityImage", "IntensityImage");
	retVal = retVal && archive.BeginTag(intensityImageTag);
	retVal = retVal && m_intensityImage.Serialize(archive);
	retVal = retVal && archive.EndTag(intensityImageTag);

	static iser::CArchiveTag amplitudeImageTag("AmplitudeImage", "AmplitudeImage");
	retVal = retVal && archive.BeginTag(amplitudeImageTag);
	retVal = retVal && m_amplitudeImage.Serialize(archive);
	retVal = retVal && archive.EndTag(amplitudeImageTag);

	static iser::CArchiveTag acquisitionParamsTag("AcquisitionParams", "AcquisitionParams");
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


