#include "isgige/CGenicamCameraSingleton.h"


// Qt includes
#include <QtNetwork/QHostAddress>


namespace isgige
{


CGenicamCameraSingleton::CGenicamCameraSingleton()
{
	gige::InitGigEVisionAPI();

	gigeApi = gige::GetGigEVisionAPI();
	if (gigeApi != NULL){
		gigeApi->RegisterCallback(this);
	}
}


CGenicamCameraSingleton::~CGenicamCameraSingleton()
{
	if (gigeApi != NULL){
		gigeApi->UnregisterCallback(this);

		gigeApi = NULL;
	}

	gige::ExitGigEVisionAPI();
}


// protected methods

// reimplemented (gige::ICallbackEvent)

void CGenicamCameraSingleton::OnConnect(gige::IDevice devicePtr)
{
	QHostAddress address(devicePtr->GetIpAddress());

	QString message = tr("Connected to camera with IP: %1").arg(address.toString());

	emit CameraEventLog(gige_EM_TYPE_INFO, message);
}


void CGenicamCameraSingleton::OnDisconnect(gige::IDevice devicePtr)
{
	QHostAddress address(devicePtr->GetIpAddress());

	QString message = tr("Disconnected from camera with IP: %1").arg(address.toString());

	emit CameraEventLog(gige_EM_TYPE_INFO, message);
}


void CGenicamCameraSingleton::OnLog(gige::IDevice /*devicePtr*/, gige::EventMessage eMessage)
{
	emit CameraEventLog(eMessage.messageType, "Internal: " + QString::fromStdString(eMessage.messageString));
}


} // namespace isgige


// include of c to cpp bridge from GigEVision SDK
#include "gige_cpp/GigEVisionSDK.cpp"


