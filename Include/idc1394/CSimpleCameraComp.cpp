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

	dc1394video_frame_t *frame=NULL;
	dc1394error_t err;
	err = dc1394_capture_dequeue(m_cameraPtr, DC1394_CAPTURE_POLICY_WAIT, &frame);
	if (err != DC1394_SUCCESS){
		SendErrorMessage(MI_CAMERA, "Could not capture a frame");

		return TS_INVALID;
	}

	int width = int(frame->size[0]);
	int height = int(frame->size[1]);

	switch (frame->data_depth){
	case 8:
		if (bitmapPtr->CreateBitmap(iimg::IBitmap::PF_GRAY, istd::CIndex2d(width, height))){
			for (int y = 0; y < height; ++y){
				const quint8* inputLinePtr = (const quint8*)bitmapPtr->GetLinePtr(y);
				quint8* ouputLinePtr = frame->image + width * y;
				std::memcpy(ouputLinePtr, inputLinePtr, width);
			}

			return TS_OK;
		}
		else{
			SendErrorMessage(MI_CAMERA, QObject::tr("Cannot create image in frame format (gray)"));
		}
		break;

	case 24:
		if (bitmapPtr->CreateBitmap(iimg::IBitmap::PF_RGB, istd::CIndex2d(width, height))){
			for (int y = 0; y < height; ++y){
				const quint8* inputLinePtr = (const quint8*)bitmapPtr->GetLinePtr(y);
				quint8* ouputLinePtr = frame->image + width * y * 3;
				for (int x = 0; x < width; ++x){
					ouputLinePtr[x * 4 + 0] = 255;
					ouputLinePtr[x * 4 + 1] = inputLinePtr[x * 3 + 0];
					ouputLinePtr[x * 4 + 2] = inputLinePtr[x * 3 + 1];
					ouputLinePtr[x * 4 + 3] = inputLinePtr[x * 3 + 2];
				}
			}

			return TS_OK;
		}
		else{
			SendErrorMessage(MI_CAMERA, QObject::tr("Cannot create image in frame format (RGB)"));
		}
		break;

	case 32:
		if (bitmapPtr->CreateBitmap(iimg::IBitmap::PF_RGBA, istd::CIndex2d(width, height))){
			for (int y = 0; y < height; ++y){
				const quint8* inputLinePtr = (const quint8*)bitmapPtr->GetLinePtr(y);
				quint8* ouputLinePtr = frame->image + width * y * 4;
				std::memcpy(ouputLinePtr, inputLinePtr, width * 4);
			}

			return TS_OK;
		}
		else{
			SendErrorMessage(MI_CAMERA, QObject::tr("Cannot create image in frame format (RGB)"));
		}
		break;

	default:
		SendErrorMessage(MI_CAMERA, QObject::tr("Camera image format is not supported"));
	}

	return TS_INVALID;
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

				err = dc1394_capture_setup(m_cameraPtr,4, DC1394_CAPTURE_FLAGS_DEFAULT);
				if (err != DC1394_SUCCESS){
					SendWarningMessage(MI_CAMERA, QObject::tr("Could not setup m_cameraPtr"));
				}

				// have the m_cameraPtr start sending us data
				err = dc1394_video_set_transmission(m_cameraPtr, DC1394_ON);
				if (err == DC1394_SUCCESS){
					SendInfoMessage(MI_CAMERA,  QObject::tr("Using m_cameraPtr with GUID %1").arg(m_cameraPtr->guid));

					return;
				}
				else{
					SendErrorMessage(MI_CAMERA, QObject::tr("Could not start m_cameraPtr iso transmission"));
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


