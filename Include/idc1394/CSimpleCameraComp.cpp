#include <idc1394/CSimpleCameraComp.h>


// Qt includes
#include <QtCore/QElapsedTimer>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iprm/IParamsSet.h>
#include <iprm/TParamsPtr.h>
#include <iimg/IBitmap.h>
#include <istd/CSystem.h>


namespace idc1394
{


CSimpleCameraComp::CSimpleCameraComp()
:	m_libraryPtr(NULL),
	m_cameraPtr(NULL)
{
}


// reimplemented (iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>)

int CSimpleCameraComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* /*inputPtr*/,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	if (m_cameraPtr == NULL){
		return TS_INVALID;
	}

	if (outputPtr == NULL){
		return TS_OK;
	}

	iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (bitmapPtr == NULL){
		return TS_INVALID;
	}

	istd::CChangeNotifier notifier(bitmapPtr);

	SetParametersToCamera(paramsPtr);

	dc1394video_frame_t* framePtr = NULL;
	dc1394error_t err;
	QElapsedTimer timer;
	timer.start();
	do{
		err = dc1394_capture_dequeue(
					m_cameraPtr,
					DC1394_CAPTURE_POLICY_POLL,
					&framePtr);
		if (err != DC1394_SUCCESS){
			SendErrorMessage(MI_CAMERA, "Could not capture a frame");

			return TS_INVALID;
		}

		if (framePtr != NULL){
			break;
		}
		else{
			istd::CSystem::Sleep(0.01);
		}
	} while (m_waitTimeoutAttrPtr.IsValid() && (!timer.hasExpired(*m_waitTimeoutAttrPtr * 1000)));

	if (framePtr == NULL){
		SendErrorMessage(MI_CAMERA, "No frames in buffer");

		return TS_INVALID;
	}

	int retVal = TS_INVALID;

	if (dc1394_capture_is_frame_corrupt(m_cameraPtr, framePtr) == DC1394_FALSE){
		int width = int(framePtr->size[0]);
		int height = int(framePtr->size[1]);

		switch (framePtr->color_coding){
		case DC1394_COLOR_CODING_MONO8:
		case DC1394_COLOR_CODING_RAW8:
			if (bitmapPtr->CreateBitmap(iimg::IBitmap::PF_GRAY, istd::CIndex2d(width, height))){
				for (int y = 0; y < height; ++y){
					const quint8* inputLinePtr = framePtr->image + width * y;
					quint8* ouputLinePtr = (quint8*)bitmapPtr->GetLinePtr(y);
					std::memcpy(ouputLinePtr, inputLinePtr, width);
				}

				retVal = TS_OK;
			}
			else{
				SendErrorMessage(MI_CAMERA, QObject::tr("Cannot create image in frame format (Gray)"));
			}
			break;

		case DC1394_COLOR_CODING_RGB8:
			if (bitmapPtr->CreateBitmap(iimg::IBitmap::PF_RGB, istd::CIndex2d(width, height))){
				for (int y = 0; y < height; ++y){
					const quint8* inputLinePtr = framePtr->image + width * y * 3;
					quint8* ouputLinePtr = (quint8*)bitmapPtr->GetLinePtr(y);
					for (int x = 0; x < width; ++x){
						ouputLinePtr[x * 4 + 3] = 255;
						ouputLinePtr[x * 4 + 2] = inputLinePtr[x * 3 + 0];
						ouputLinePtr[x * 4 + 1] = inputLinePtr[x * 3 + 1];
						ouputLinePtr[x * 4 + 0] = inputLinePtr[x * 3 + 2];
					}
				}

				retVal = TS_OK;
			}
			else{
				SendErrorMessage(MI_CAMERA, QObject::tr("Cannot create image in frame format (RGB)"));
			}
			break;

		case DC1394_COLOR_CODING_MONO16:
		case DC1394_COLOR_CODING_RAW16:
			if (*m_only8BitDepthAttrPtr){
				int inputOffset = (framePtr->little_endian != 0)? 1: 0;
				if (bitmapPtr->CreateBitmap(iimg::IBitmap::PF_GRAY, istd::CIndex2d(width, height))){
					for (int y = 0; y < height; ++y){
						const quint8* inputLinePtr = framePtr->image + width * y * 2 + inputOffset;
						quint8* ouputLinePtr = (quint8*)bitmapPtr->GetLinePtr(y);
						for (int x = 0; x < width; ++x){
							ouputLinePtr[x] = inputLinePtr[x * 2];
						}
					}

					retVal = TS_OK;
				}
				else{
					SendErrorMessage(MI_CAMERA, QObject::tr("Cannot create image in frame format (Gray)"));
				}
			}
			else{
				if (bitmapPtr->CreateBitmap(iimg::IBitmap::PF_GRAY16, istd::CIndex2d(width, height))){
					for (int y = 0; y < height; ++y){
						const quint8* inputLinePtr = framePtr->image + width * y * 2;
						quint8* ouputLinePtr = (quint8*)bitmapPtr->GetLinePtr(y);
						std::memcpy(ouputLinePtr, inputLinePtr, width * 2);
					}

					retVal = TS_OK;
				}
				else{
					SendErrorMessage(MI_CAMERA, QObject::tr("Cannot create image in frame format (Gray16)"));
				}
			}
			break;

		default:
			SendErrorMessage(MI_CAMERA, QObject::tr("Camera image format is not supported"));
		}
	}

	err = dc1394_capture_enqueue(m_cameraPtr, framePtr);
	if (err != DC1394_SUCCESS){
		SendWarningMessage(MI_CAMERA, "Camera ring buffer error");
	}

	return retVal;
}


// reimplemented (icam::IBitmapAcquisition)

istd::CIndex2d CSimpleCameraComp::GetBitmapSize(const iprm::IParamsSet* /*paramsPtr*/) const
{
	return istd::CIndex2d::GetInvalid();
}


// reimplemented (iproc::IProcessor)

void CSimpleCameraComp::InitProcessor(const iprm::IParamsSet* paramsPtr)
{
	SetParametersToCamera(paramsPtr);
}


// protected methods

void CSimpleCameraComp::SetParametersToCamera(const iprm::IParamsSet* paramsPtr)
{
	if (m_cameraPtr != NULL){
		iprm::TParamsPtr<imeas::ILinearAdjustParams> adjustParamsPtr(paramsPtr, m_adjustParamsIdAttrPtr, m_defaultAdjustParamsCompPtr, false);
		if (adjustParamsPtr.IsValid()){
			SetAbsoluteFeatureValue(*m_cameraPtr, DC1394_FEATURE_GAIN, adjustParamsPtr->GetScaleFactor());
			SetAbsoluteFeatureValue(*m_cameraPtr, DC1394_FEATURE_BRIGHTNESS, adjustParamsPtr->GetOffsetFactor());
		}

		iprm::TParamsPtr<icam::IExposureParams> exposureParamsPtr(paramsPtr, m_exposureParamsIdAttrPtr, m_defaultExposureParamsCompPtr, false);
		if (exposureParamsPtr.IsValid()){
			SetAbsoluteFeatureValue(*m_cameraPtr, DC1394_FEATURE_SHUTTER, exposureParamsPtr->GetShutterTime());
		}
	}
}


bool CSimpleCameraComp::GetAbsoluteFeatureRange(dc1394camera_t& camera, dc1394feature_t feature, istd::CRange& range) const
{
	dc1394bool_t flag;
	if (		(dc1394_feature_has_absolute_control(&camera, feature, &flag) == DC1394_SUCCESS) &&
				(flag == DC1394_TRUE)){
		float minValue = 0;
		float maxValue = 0;
		if (dc1394_feature_get_absolute_boundaries(&camera, feature, &minValue, &maxValue) == DC1394_SUCCESS){
			range = istd::CRange(qMin(minValue, maxValue), qMax(minValue, maxValue));

			return true;
		}
	}

	return false;
}


bool CSimpleCameraComp::SetAbsoluteFeatureValue(dc1394camera_t& camera, dc1394feature_t feature, double value)
{
	dc1394bool_t flag;
	if (		(dc1394_feature_has_absolute_control(&camera, feature, &flag) == DC1394_SUCCESS) &&
				(flag == DC1394_TRUE)){
		if (dc1394_feature_set_absolute_value(&camera, feature, float(value)) == DC1394_SUCCESS){
			return true;
		}
	}

	return false;
}


// reimplemented (icomp::CComponentBase)

void CSimpleCameraComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	dc1394camera_list_t * list;

	dc1394error_t err;

	m_adjustConstraints.scaleRange = istd::CRange::GetNull();
	m_adjustConstraints.offsetRange = istd::CRange::GetNull();

	m_exposureConstraints.shutterTimeRange = istd::CRange::GetNull();

	m_libraryPtr = dc1394_new ();
	if (m_libraryPtr == NULL){
		SendCriticalMessage(MI_CAMERA, QObject::tr("Could not initialize library"));

		return;
	}

	err = dc1394_camera_enumerate (m_libraryPtr, &list);
	if (err == DC1394_SUCCESS){
		if (list->num > 0){
			m_cameraPtr = dc1394_camera_new(m_libraryPtr, list->ids[0].guid);
			if (m_cameraPtr != NULL){
				GetAbsoluteFeatureRange(*m_cameraPtr, DC1394_FEATURE_SHUTTER, m_exposureConstraints.shutterTimeRange);
				GetAbsoluteFeatureRange(*m_cameraPtr, DC1394_FEATURE_GAIN, m_adjustConstraints.scaleRange);
				GetAbsoluteFeatureRange(*m_cameraPtr, DC1394_FEATURE_BRIGHTNESS, m_adjustConstraints.offsetRange);

				// turn automatic exposure off
				dc1394_feature_set_mode(m_cameraPtr, DC1394_FEATURE_SHUTTER, DC1394_FEATURE_MODE_MANUAL);
				dc1394_feature_set_mode(m_cameraPtr, DC1394_FEATURE_EXPOSURE, DC1394_FEATURE_MODE_MANUAL);
				dc1394_feature_set_mode(m_cameraPtr, DC1394_FEATURE_GAIN, DC1394_FEATURE_MODE_MANUAL);
				dc1394_feature_set_mode(m_cameraPtr, DC1394_FEATURE_BRIGHTNESS, DC1394_FEATURE_MODE_MANUAL);
				dc1394_feature_set_absolute_control(m_cameraPtr, DC1394_FEATURE_EXPOSURE, DC1394_ON);
				dc1394_feature_set_absolute_control(m_cameraPtr, DC1394_FEATURE_GAIN, DC1394_ON);
				dc1394_feature_set_absolute_control(m_cameraPtr, DC1394_FEATURE_BRIGHTNESS, DC1394_ON);

				// setup capture
				err = dc1394_video_set_iso_speed(m_cameraPtr, DC1394_ISO_SPEED_400);
				if (err != DC1394_SUCCESS){
					SendWarningMessage(MI_CAMERA, QObject::tr("Could not set ISO speed"));
				}

				err = dc1394_video_set_mode(m_cameraPtr, DC1394_VIDEO_MODE_640x480_RGB8);
				if (err != DC1394_SUCCESS){
					SendWarningMessage(MI_CAMERA, QObject::tr("Could not set video mode"));
				}

				err = dc1394_video_set_framerate(m_cameraPtr, DC1394_FRAMERATE_7_5);
				if (err != DC1394_SUCCESS){
					SendWarningMessage(MI_CAMERA, QObject::tr("Could not set frame rate"));
				}

				err = dc1394_capture_setup(m_cameraPtr, *m_bufferSizeAttrPtr, DC1394_CAPTURE_FLAGS_DEFAULT);
				if (err != DC1394_SUCCESS){
					SendErrorMessage(MI_CAMERA, QObject::tr("Could not setup camera"));
				}
				else{
					// have the m_cameraPtr start sending us data
					err = dc1394_video_set_transmission(m_cameraPtr, DC1394_ON);
					if (err == DC1394_SUCCESS){
						SendInfoMessage(MI_CAMERA, QObject::tr("Using camera with GUID %1").arg(m_cameraPtr->guid));

						return;
					}
					else{
						SendErrorMessage(MI_CAMERA, QObject::tr("Could not start camera ISO transmission"));
					}
				}

				dc1394_camera_free(m_cameraPtr);

				m_cameraPtr = NULL;
			}
			else{
				SendErrorMessage(MI_CAMERA, QObject::tr("Failed to initialize camera with guid %1").arg(list->ids[0].guid));
			}
		}
		else{
			SendErrorMessage(MI_CAMERA, QObject::tr("No cameras found"));
		}

		dc1394_camera_free_list(list);
	}
	else{
		SendCriticalMessage(MI_CAMERA, QObject::tr("Failed to enumerate cameras"));
	}
}


void CSimpleCameraComp::OnComponentDestroyed()
{
	if (m_libraryPtr != NULL){
		if (m_cameraPtr != NULL){
			dc1394_video_set_transmission(m_cameraPtr, DC1394_OFF);
			dc1394_capture_stop(m_cameraPtr);
			dc1394_camera_free(m_cameraPtr);
		}

		dc1394_free (m_libraryPtr);
	}

	BaseClass::OnComponentDestroyed();
}


// public methods of embedded class AdjustConstraints

// reimplemented (imeas::ILinearAdjustConstraints)

istd::CRange CSimpleCameraComp::AdjustConstraints::GetScaleFactorRange() const
{
	return scaleRange;
}


istd::CRange CSimpleCameraComp::AdjustConstraints::GetOffsetFactorRange() const
{
	return offsetRange;
}


// public methods of embedded class ExposureConstraints

// reimplemented (icam::IExposureConstraints)

istd::CRange CSimpleCameraComp::ExposureConstraints::GetShutterTimeRange() const
{
	return shutterTimeRange;
}


istd::CRange CSimpleCameraComp::ExposureConstraints::GetDelayTimeRange() const
{
	return istd::CRange::GetNull();
}


istd::CRange CSimpleCameraComp::ExposureConstraints::GetEenDelayRange() const
{
	return istd::CRange::GetNull();
}


} // namespace idc1394


