#include "iswr/CSwissRangerAcquisitionComp.h"

#include "istd/TChangeNotifier.h"

#include "iprm/IParamsSet.h"

#include "iimg/CGeneralBitmap.h"

#include "iswr/ISwissRangerParams.h"


namespace iswr
{


CSwissRangerAcquisitionComp::CSwissRangerAcquisitionComp()
:	m_cameraPtr(NULL)
{
}


bool CSwissRangerAcquisitionComp::IsCameraValid() const
{
	return (m_cameraPtr != NULL);
}


// reimplemented (iproc::TSyncProcessorWrap<iproc::IBitmapAcquisition>)

int CSwissRangerAcquisitionComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* /*inputPtr*/,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* /*progressManagerPtr*/)
{
	if (!IsCameraValid()){
		return TS_INVALID;
	}

	const iswr::ISwissRangerParams* swissRangerParamsPtr = NULL;
	if (paramsPtr != NULL && m_swissRangerParamsIdAttrPtr.IsValid()){
		swissRangerParamsPtr = dynamic_cast<const iswr::ISwissRangerParams*>(
			paramsPtr->GetParameter((*m_swissRangerParamsIdAttrPtr).ToString()));
	}


	if (swissRangerParamsPtr != NULL){
		int currentCameraMode = SR_GetMode(m_cameraPtr);

		if(swissRangerParamsPtr->IsMedianFilterEnabled()){
			currentCameraMode = currentCameraMode  | AM_MEDIAN;
		}
		else{
			currentCameraMode = currentCameraMode & ~AM_MEDIAN;
		}

		if (swissRangerParamsPtr->IsAdaptiveFilterEnabled()){
			currentCameraMode = currentCameraMode  | AM_DENOISE_ANF;
		}
		else{
			currentCameraMode = currentCameraMode & ~AM_DENOISE_ANF;
		}

		SR_SetMode(m_cameraPtr, currentCameraMode);		
		
		SR_SetModulationFrequency(m_cameraPtr, ModulationFrq(swissRangerParamsPtr->GetModulationFrequencyMode()));

		I_WORD amplitudeThreshold = I_WORD(swissRangerParamsPtr->GetAmplitudeThreshold() * (1 << 16)); 
		SR_SetAmplitudeThreshold(m_cameraPtr, amplitudeThreshold);
	}

	// setup exposure params:
	const icam::IExposureParams* exposureParamsPtr = NULL;
	if (paramsPtr != NULL && m_exposureParamsIdAttrPtr.IsValid()){
		exposureParamsPtr = dynamic_cast<const icam::IExposureParams*>(
					paramsPtr->GetParameter((*m_exposureParamsIdAttrPtr).ToString()));
	}

	if (exposureParamsPtr == NULL && m_defaultExposureParamsCompPtr.IsValid()){
		exposureParamsPtr = m_defaultExposureParamsCompPtr.GetPtr();
	}

	if (exposureParamsPtr != NULL){
		double shutterTime = exposureParamsPtr->GetShutterTime();
		I_ASSERT(GetShutterTimeRange().Contains(shutterTime));

		unsigned char integrationTime = unsigned char((shutterTime - 0.0003) / 0.0001);

		SR_SetIntegrationTime(m_cameraPtr, integrationTime);
	}

	static float maxDistances[MF_LAST] = {
				3.75f,
				5.0f,
				7.14f, 
				7.5f,
				7.895f, 
				2.5f, 
				10.f, 
				15.f, 
				5.17f, 
				4.8487f,
				10.34f,
				9.68f
	};
	
	ModulationFrq frq = SR_GetModulationFrequency(m_cameraPtr);
	float maxDistance = maxDistances[frq];

	// acquire camera image:
	SR_Acquire(m_cameraPtr);

	// create output:
	if (outputPtr != NULL){
		SR_CoordTrfUint16(
			m_cameraPtr, 
			&m_xBuffer[0], 
			&m_yBuffer[0], 
			&m_zBuffer[0], 
			sizeof(I_SWORD),
			sizeof(I_SWORD),
			sizeof(I_WORD));

		istd::TChangeNotifier<iimg::IBitmap> bitmapPtr(dynamic_cast<iimg::IBitmap*>(outputPtr));
		if (bitmapPtr.IsValid()){
			ImgEntry* imgEntryArray = NULL;
			int imageCount = SR_GetImageList(m_cameraPtr, &imgEntryArray);
	
			for (int imageIndex = 0; imageIndex < imageCount; imageIndex++){
				ImgEntry currentImage = imgEntryArray[imageIndex];
				if (currentImage.imgType == ImgEntry::IT_DISTANCE){
					CreateFromCamera(*dynamic_cast<iimg::IBitmap*>(outputPtr), currentImage, int(maxDistance * 1000));
				}
			}
		}
		else{
			istd::TChangeNotifier<iswr::ISwissRangerAcquisitionData> swissImagePtr(dynamic_cast<iswr::ISwissRangerAcquisitionData*>(outputPtr));
			if (swissImagePtr.IsValid()){
				if (!CreateSwissImage(*swissImagePtr.GetPtr(), maxDistance)){
					return TS_INVALID;
				}
			}
			else{
				return TS_INVALID;
			}
		}
	}

	return TS_OK;
}


// reimplemented (iproc::IBitmapAcquisition)

istd::CIndex2d CSwissRangerAcquisitionComp::GetBitmapSize(const iprm::IParamsSet* /*paramsPtr*/) const
{
	if (IsCameraValid()){
		int width = SR_GetCols(m_cameraPtr);
		int height = SR_GetRows(m_cameraPtr);

		return istd::CIndex2d(width, height);
	}

	return istd::CIndex2d(-1, -1);	// unknown size
}


// reimplemented (iswr::ISwissRangerConstrains)

const ISwissRangerConstrains::SupportedFrequencies& CSwissRangerAcquisitionComp::GetSupportedFrequences() const
{
	return m_supportedFrequencies;
}


// reimplemented (icam::IExposureConstraints)

istd::CRange CSwissRangerAcquisitionComp::GetShutterTimeRange() const
{
	return istd::CRange(0.0003, 0.0258);
}


istd::CRange CSwissRangerAcquisitionComp::GetDelayTimeRange() const
{
	return istd::CRange(-1, -1);
}


istd::CRange CSwissRangerAcquisitionComp::GetEenDelayRange() const
{
	return istd::CRange(-1, -1);
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CSwissRangerAcquisitionComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	// create camera device and open it:
	int deviceId = SR_OpenUSB(&m_cameraPtr, 0);
	if (deviceId != 0){
		I_ASSERT(m_cameraPtr != NULL);
		I_ASSERT(m_timeoutAttrPtr.IsValid());

		int timeout = int(*m_timeoutAttrPtr * 1000);
		SR_SetTimeout(m_cameraPtr, timeout);
	
		SR_SetMode(m_cameraPtr, AM_CONF_MAP | AM_COR_FIX_PTRN);
		SR_SetModulationFrequency(m_cameraPtr, MF_31MHz);
		SR_SetIntegrationTime(m_cameraPtr, 128);

		istd::CIndex2d size = GetBitmapSize(NULL);
		if (!size.IsSizeEmpty()){
			m_xBuffer.resize(size.GetProductVolume());
			m_yBuffer.resize(size.GetProductVolume());
			m_zBuffer.resize(size.GetProductVolume());
		}

		// set device info
		if (m_deviceInfoCompPtr.IsValid()){
			char deviceString[4096] = {0};

			SR_GetDeviceString(m_cameraPtr, deviceString, 4096);

			m_deviceInfoCompPtr->SetDeviceId(deviceString);
		}
	}
	else{
		SendErrorMessage(0, "Could not open USB inteface", "SwissRanger");
	}
}


void CSwissRangerAcquisitionComp::OnComponentDestroyed()
{
	if (IsCameraValid()){
		SR_Close(m_cameraPtr);
		
		m_cameraPtr = NULL;
	}

	BaseClass::OnComponentDestroyed();
}


// private methods

bool CSwissRangerAcquisitionComp::CreateSwissImage(
			iswr::ISwissRangerAcquisitionData& swissImage,
			double maxDistance) const
{
	if (!m_zBuffer.empty()){
		I_ASSERT(!m_xBuffer.empty());
		I_ASSERT(!m_yBuffer.empty());

		istd::CIndex2d size = GetBitmapSize(NULL);

		int maxDistanceMm = int(maxDistance * 1000);

		istd::TDelPtr<I_WORD, istd::ArrayAccessor<I_WORD> > imageDataPtr(new I_WORD[size.GetProductVolume()]);

		for (int y = 0; y < size.GetY(); ++y){
			I_WORD* outputImageLinePtr = imageDataPtr.GetPtr() + y * size.GetX();
			const I_WORD* zBufferPtr = &m_zBuffer[y * size.GetX()];
		
			for (int x = 0; x < size.GetX(); x++){
				I_WORD zValue = zBufferPtr[x];
			
				double normedZValue = zValue / (double)maxDistanceMm;
				outputImageLinePtr[x] = I_WORD(normedZValue * maxDistanceMm); // depth in mm
			}
		}

		iimg::CGeneralBitmap depthImage;
		if (!depthImage.CreateBitmap(iimg::IBitmap::PF_GRAY16, size, imageDataPtr.PopPtr(), true, size.GetX() * 2)){
			return false;
		}

		ImgEntry* imgEntryArray = NULL;

		iimg::CGeneralBitmap amplitudeImage;
		iimg::CGeneralBitmap intensityImage;
		iimg::CGeneralBitmap confidenceMap;

		int imageCount = SR_GetImageList(m_cameraPtr, &imgEntryArray);
		for (int imageIndex = 0; imageIndex < imageCount; imageIndex++){
			ImgEntry currentImage = imgEntryArray[imageIndex];

			if (currentImage.imgType == ImgEntry::IT_AMPLITUDE){
				CreateFromCamera(amplitudeImage, currentImage, 0xffff);
			}
			if (currentImage.imgType == ImgEntry::IT_DISTANCE){
				CreateFromCamera(intensityImage, currentImage, 0xffff);
			}
			if (currentImage.imgType == ImgEntry::IT_CONF_MAP){
				CreateFromCamera(confidenceMap, currentImage, 0xffff);
			}
		}		

		return swissImage.CreateData(
					maxDistanceMm,
					depthImage,
					confidenceMap,
					intensityImage,
					amplitudeImage,
					&m_xBuffer[0],
					&m_yBuffer[0]);
	}

	return false;
}

	
bool CSwissRangerAcquisitionComp::CreateFromCamera(iimg::IBitmap& image, const ImgEntry& imageEntry, int scaleFactor) const
{
	istd::TDelPtr<I_BYTE, istd::ArrayAccessor<I_BYTE> > imageDataPtr(new I_BYTE[imageEntry.width * imageEntry.height]);

	// convert input data to 8-bit image:
	for (int y = 0; y < imageEntry.height; y++){
		I_BYTE* imageLinePtr = imageDataPtr.GetPtr() + y * imageEntry.width;
		const I_WORD* inputDataPtr = (I_WORD*)imageEntry.data + y * imageEntry.width;
		for (int x = 0; x < imageEntry.width; x++){
			imageLinePtr[x] = I_BYTE((inputDataPtr[x] / (double)scaleFactor) * 255);
		}
	}

	return image.CreateBitmap(
				iimg::IBitmap::PF_GRAY,
				istd::CIndex2d(imageEntry.width, imageEntry.height), 
				imageDataPtr.PopPtr(),
				true);
}


} // namespace iswr


