#include "idc1394/CSimpleCameraComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "iprm/IParamsSet.h"
#include "iimg/IBitmap.h"


namespace idc1394
{


CSimpleCameraComp::CSimpleCameraComp()
:	m_libraryPtr(NULL),
	m_cameraPtr(NULL)
{
}


// reimplemented (iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>)

int CSimpleCameraComp::DoProcessing(
			const iprm::IParamsSet* /* paramsPtr*/,
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

	istd::TChangeNotifier<iimg::IBitmap> bitmapPtr(dynamic_cast<iimg::IBitmap*>(outputPtr));
	if (!bitmapPtr.IsValid()){
		return TS_INVALID;
	}

	dc1394video_frame_t* framePtr = NULL;
	dc1394error_t err;
	err = dc1394_capture_dequeue(
				m_cameraPtr,
				*m_waitForFrameAttrPtr? DC1394_CAPTURE_POLICY_WAIT: DC1394_CAPTURE_POLICY_POLL,
				&framePtr);
	if (err != DC1394_SUCCESS){
		SendErrorMessage(MI_CAMERA, "Could not capture a frame");

		return TS_INVALID;
	}

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


// protected methods

// reimplemented (icomp::CComponentBase)

void CSimpleCameraComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	dc1394camera_list_t * list;

	dc1394error_t err;

	m_libraryPtr = dc1394_new ();
	if (m_libraryPtr == NULL){
		SendCriticalMessage(MI_CAMERA, QObject::tr("Could not initialize library"));

		return;
	}

	err = dc1394_camera_enumerate (m_libraryPtr, &list);
	if (err == DC1394_SUCCESS){
		if (list->num > 0){
			m_cameraPtr = dc1394_camera_new (m_libraryPtr, list->ids[0].guid);
			if (m_cameraPtr != NULL){
				// turn automatic exposure off
				dc1394_feature_set_mode(m_cameraPtr, DC1394_FEATURE_SHUTTER, DC1394_FEATURE_MODE_MANUAL);
				dc1394_feature_set_mode(m_cameraPtr, DC1394_FEATURE_EXPOSURE, DC1394_FEATURE_MODE_MANUAL);
				dc1394_feature_set_mode(m_cameraPtr, DC1394_FEATURE_GAIN, DC1394_FEATURE_MODE_MANUAL);
//				dc1394_feature_set_absolute_control(m_cameraPtr, DC1394_FEATURE_SHUTTER, DC1394_ON);
				dc1394_feature_set_absolute_control(m_cameraPtr, DC1394_FEATURE_EXPOSURE, DC1394_ON);
//				dc1394_feature_set_absolute_control(m_cameraPtr, DC1394_FEATURE_GAIN, DC1394_ON);
				dc1394_feature_set_absolute_value(m_cameraPtr, DC1394_FEATURE_EXPOSURE, 2000.0);

				// setup capture
				err = dc1394_video_set_iso_speed(m_cameraPtr, DC1394_ISO_SPEED_400);
				if (err != DC1394_SUCCESS){
					SendWarningMessage(MI_CAMERA, QObject::tr("Could not set iso speed"));
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
						SendErrorMessage(MI_CAMERA, QObject::tr("Could not start m_cameraPtr iso transmission"));
					}
				}

				dc1394_camera_free(m_cameraPtr);

				m_cameraPtr = NULL;
			}
			else{
				SendErrorMessage(MI_CAMERA, QObject::tr("Failed to initialize m_cameraPtr with guid %1").arg(list->ids[0].guid));
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


} // namespace idc1394


