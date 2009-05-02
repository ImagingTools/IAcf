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


// reimplemented (iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>)

int CSwissRangerAcquisitionComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* /*inputPtr*/,
			istd::IChangeable* outputPtr)
{
	if (!IsCameraValid()){
		return TS_INVALID;
	}

	const iswr::ISwissRangerParams* swissRangerParamsPtr = NULL;
	if (paramsPtr != NULL && m_swissRangerParamsIdAttrPtr.IsValid()){
		swissRangerParamsPtr = dynamic_cast<const iswr::ISwissRangerParams*>(
			paramsPtr->GetParameter((*m_swissRangerParamsIdAttrPtr).ToString()));
	}

	istd::CRange clippingDistanceRange(0, 1.0);

	if (swissRangerParamsPtr != NULL){
		int currentCameraMode = SR_GetMode(m_cameraPtr);

		if(!swissRangerParamsPtr->IsMedianFilterEnabled()){
			currentCameraMode = currentCameraMode & ~AM_MEDIAN;
		}
		else{
			currentCameraMode = currentCameraMode  | AM_MEDIAN;
		}

		clippingDistanceRange = swissRangerParamsPtr->GetDistanceClipRange();

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

		unsigned char integrationTime = unsigned char(shutterTime / 0.0002 - 1);

		SR_SetIntegrationTime(m_cameraPtr, integrationTime);
	}

	static float maxDistances[MF_LAST] = {
				3.75f,
				5.0f,
				7.142f, 
				7.5f,
				7.895f, 
				2.6f, 
				10.f, 
				15.f, 
				5.1724f, 
				4.8387f};//MF_40MHz,MF_30MHz,MF_21MHz,MF_20MHz,MF_19MHz,...
	
	ModulationFrq frq = SR_GetModulationFrequency(m_cameraPtr);
	float maxDistance = maxDistances[frq];

	// acquire camera image:
	SR_Acquire(m_cameraPtr);

	// create output:
	if (outputPtr != NULL){
		SR_CoordTrfDbl(
			m_cameraPtr, 
			m_xBuffer.GetPtr(), 
			m_yBuffer.GetPtr(), 
			m_zBuffer.GetPtr(), 
			sizeof(double),
			sizeof(double),
			sizeof(double));

		istd::TChangeNotifier<iimg::IBitmap> bitmapPtr(dynamic_cast<iimg::IBitmap*>(outputPtr));
		if (bitmapPtr.IsValid()){
			if (!CreateOutputBitmap(*bitmapPtr.GetPtr(), maxDistance, clippingDistanceRange)){
				return TS_INVALID;
			}
		}
		else{
			iimg::CGeneralBitmap amplitudeBitmap;
			if (CreateOutputBitmap(amplitudeBitmap, maxDistance, clippingDistanceRange)){
				istd::TChangeNotifier<iswr::ISwissRangerImage> swissImagePtr(dynamic_cast<iswr::ISwissRangerImage*>(outputPtr));
				if (swissImagePtr.IsValid()){
					if (!CreateSwissImage(*swissImagePtr.GetPtr(), amplitudeBitmap)){
						return TS_INVALID;
					}
				}
			}
		}
	}

	return TS_OK;
}


// reimplemented (icam::IBitmapAcquisition)

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
	return istd::CRange(0.0002, 0.051);
}


istd::CRange CSwissRangerAcquisitionComp::GetDelayTimeRange() const
{
	return istd::CRange(-1, -1);
}


istd::CRange CSwissRangerAcquisitionComp::GetEenDelayRange() const
{
	return istd::CRange(-1, -1);
}


// reimplemented (icomp::IComponent)

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
	
		SR_SetMode(m_cameraPtr, AM_CONF_MAP | AM_MEDIAN | AM_COR_FIX_PTRN | AM_CONV_GRAY);
		SR_SetModulationFrequency(m_cameraPtr, MF_40MHz);
		SR_SetIntegrationTime(m_cameraPtr, 128);

		m_imagesCount = SR_GetImageList(m_cameraPtr, &m_imgEntryArray);

		istd::CIndex2d size = GetBitmapSize(NULL);
		if (!size.IsSizeEmpty()){
			m_xBuffer.SetPtr(new double[size.GetProductVolume()]);
			m_yBuffer.SetPtr(new double[size.GetProductVolume()]);
			m_zBuffer.SetPtr(new double[size.GetProductVolume()]);
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

bool CSwissRangerAcquisitionComp::CreateOutputBitmap(
			iimg::IBitmap& bitmap, 
			double maxDistance, 
			const istd::CRange& clippingDistanceRange) const
{
	bool retVal = false;

	istd::CIndex2d size = GetBitmapSize(NULL);
	if (!size.IsSizeEmpty()){
		if (bitmap.CreateBitmap(size)){
			for (int y = 0; y < size.GetY(); ++y){
				I_BYTE* outputBitmapPtr = (I_BYTE*)bitmap.GetLinePtr(y);
				double* zBufferPtr = m_zBuffer.GetPtr() + y * size.GetX();

				for (int x = 0; x < size.GetX(); x++){
					double zValue = *(zBufferPtr + x);
					I_ASSERT(zValue >= 0.0f);
					I_ASSERT(zValue <= maxDistance);

					double normedZValue = zValue / maxDistance;
					normedZValue = clippingDistanceRange.GetClipped(normedZValue);
					normedZValue = clippingDistanceRange.GetMappedTo(normedZValue, istd::CRange(0, 1));
					outputBitmapPtr[x] = I_BYTE(normedZValue * 255.0);
				}
			}

			retVal = true;
		}
	}

	return retVal;
}


bool CSwissRangerAcquisitionComp::CreateSwissImage(
			iswr::ISwissRangerImage& swissImage,
			iimg::IBitmap& amplitudeBitmap) const
{
	if (m_zBuffer.IsValid()){
		istd::CIndex2d size = GetBitmapSize(NULL);

		return swissImage.CreateImage(m_zBuffer.GetPtr(), amplitudeBitmap);
	}

	return false;
}


} // namespace iswr


