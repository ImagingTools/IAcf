#ifndef isgige_CGenicamCameraSingleton_included
#define isgige_CGenicamCameraSingleton_included


// Qt includes
#include <QObject>

// GigE includes
#include "gige_cpp/GigEVisionSDK.h"
#include "gige_cpp/ICallbackEvent.h"


namespace isgige
{


class CGenicamCameraComp;


class CGenicamCameraSingleton:
			public QObject,
			protected gige::ICallbackEvent
{
	Q_OBJECT

public:
	CGenicamCameraSingleton();
	virtual ~CGenicamCameraSingleton();

	gige::IGigEVisionAPI gigeApi;

signals:
	void CameraEventLog(int type, QString message);

protected:
	// reimplemented (gige::ICallbackEvent)
	virtual void OnConnect(gige::IDevice devicePtr);
	virtual void OnDisconnect(gige::IDevice devicePtr);
	virtual void OnLog(gige::IDevice devicePtr, gige::EventMessage eMessage);
};


} // namespace isgige


#endif //! isgige_CGenicamCameraSingleton_included


