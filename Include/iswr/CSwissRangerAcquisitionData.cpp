#include "iswr/CSwissRangerAcquisitionData.h"


// STL includes
#include <cstring>

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
:	m_maxDepth(0)
{
}


// reimplemented (iswr::ISwissRangerAcquisitionData)

bool CSwissRangerAcquisitionData::CreateData(
			int maximalDepth,
			const iimg::IBitmap& depthImage,
			const iimg::IBitmap& confidenceMap,
			const iimg::IBitmap& intensityImage,
			const iimg::IBitmap& amplitudeImage,
			const I_SWORD* xBufferPtr,
			const I_SWORD* yBufferPtr)
{
	m_maxDepth = maximalDepth;

	m_depthImage.CopyFrom(depthImage);
	m_confidenceMap.CopyFrom(confidenceMap);
	m_intensityImage.CopyFrom(intensityImage);
	m_amplitudeImage.CopyFrom(amplitudeImage);

	int imageDataSize = m_depthImage.GetImageSize().GetX() * m_depthImage.GetImageSize().GetY();

	m_xBuffer.resize(imageDataSize);
	std::memcpy(&m_xBuffer[0], xBufferPtr, imageDataSize * sizeof(I_SWORD));

	m_yBuffer.resize(imageDataSize);
	std::memcpy(&m_yBuffer[0], yBufferPtr, imageDataSize * sizeof(I_SWORD));

	return true;
}


double CSwissRangerAcquisitionData::GetMaxDistance() const
{
	return m_maxDepth;
}


const iimg::IBitmap& CSwissRangerAcquisitionData::GetDistanceImage() const
{
	// calculate the 8-bit version of depth image.
	if (m_distanceImage.IsEmpty() && !m_depthImage.IsEmpty()){
		int imageWidth = m_depthImage.GetImageSize().GetX();
		int imageHeight = m_depthImage.GetImageSize().GetY();

		istd::TDelPtr<I_BYTE, istd::ArrayAccessor<I_BYTE> > imageDataPtr(new I_BYTE[imageWidth * imageHeight]);

		// convert input data to 8-bit image:
		for (int y = 0; y < imageHeight; y++){
			I_BYTE* imageLinePtr = imageDataPtr.GetPtr() + y * imageWidth;
			I_WORD* inputDataPtr = (I_WORD*)m_depthImage.GetLinePtr(y);
			for (int x = 0; x < imageWidth; x++){
				imageLinePtr[x] = I_BYTE((inputDataPtr[x] / (double)m_maxDepth) * 255);
			}
		}

		m_distanceImage.CreateBitmap(m_depthImage.GetImageSize(), imageDataPtr.PopPtr(), true);
	}
	
	return m_distanceImage;
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


const I_SWORD* CSwissRangerAcquisitionData::GetXCoordinatesBuffer() const
{
	return &m_xBuffer[0];
}


const I_SWORD* CSwissRangerAcquisitionData::GetYCoordinatesBuffer() const
{
	return &m_yBuffer[0];
}


// reimplemented (iser::ISerializable)


bool CSwissRangerAcquisitionData::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	istd::CChangeNotifier changePtr(archive.IsStoring() ? NULL : this);

	static iser::CArchiveTag depthImageTag("DepthImage", "DepthImage");
	retVal = retVal && archive.BeginTag(depthImageTag);
	retVal = retVal && m_depthImage.Serialize(archive);
	retVal = retVal && archive.EndTag(depthImageTag);

	static iser::CArchiveTag maxDepthTag("MaxDepth", "MaxDepth");
	retVal = retVal && archive.BeginTag(maxDepthTag);
	retVal = retVal && archive.Process(m_maxDepth);
	retVal = retVal && archive.EndTag(maxDepthTag);

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

	int imageDataSize = m_depthImage.GetImageSize().GetX() * m_depthImage.GetImageSize().GetY();
	if (!archive.IsStoring()){
		m_xBuffer.resize(imageDataSize);
		m_yBuffer.resize(imageDataSize);
	}

	static iser::CArchiveTag xBufferTag("XBuffer", "X - coordinates");
	retVal = retVal && archive.BeginTag(xBufferTag);
	retVal = retVal && archive.ProcessData(&m_xBuffer[0], imageDataSize * sizeof(I_SWORD));
	retVal = retVal && archive.EndTag(xBufferTag);

	static iser::CArchiveTag yBufferTag("YBuffer", "Y - coordinates");
	retVal = retVal && archive.BeginTag(yBufferTag);
	retVal = retVal && archive.ProcessData(&m_yBuffer[0], imageDataSize * sizeof(I_SWORD));
	retVal = retVal && archive.EndTag(yBufferTag);

	if (!archive.IsStoring()){
		m_distanceImage.ResetImage();

		GetDistanceImage();
	}

	return retVal;
}


} // namespace iswr


