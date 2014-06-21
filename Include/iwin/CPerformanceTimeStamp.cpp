#include "iwin/CPerformanceTimeStamp.h"


// Windows includes
#include <windows.h>

// ACF includes
#include "istd/CChangeNotifier.h"


namespace iwin
{


CPerformanceTimeStamp::CPerformanceTimeStamp()
{
	m_startCounter = 0;

	if (s_isTimerFrequenceValid){
		Start();
	}
}


CPerformanceTimeStamp::CPerformanceTimeStamp(quint64 nativeValue)
{
	m_startCounter = qint64(nativeValue);
}


bool CPerformanceTimeStamp::IsVaild() const
{
	return s_isTimerFrequenceValid;
}


double CPerformanceTimeStamp::GetTimeTo(const CPerformanceTimeStamp& timeStamp) const
{
	return (m_startCounter - timeStamp.m_startCounter) / double(s_timerFrequence);
}


quint64 CPerformanceTimeStamp::GetNativeRepresentation() const
{
	return quint64(m_startCounter);
}


void CPerformanceTimeStamp::SetNativeRepresentation(quint64 value)
{
	static ChangeSet changeSet(CF_START_SET);
	istd::CChangeNotifier notifier(this, changeSet);

	m_startCounter = qint64(value);
}


// reimplemented (istd::ITimeStamp)

void CPerformanceTimeStamp::Start(double elapsedTime)
{
	Q_ASSERT(sizeof(qint64) == sizeof(LARGE_INTEGER));

	LARGE_INTEGER currentCounter;
	::QueryPerformanceCounter(&currentCounter);

	SetNativeRepresentation((quint64&)currentCounter - quint64(elapsedTime * s_timerFrequence));
}


QDateTime CPerformanceTimeStamp::GetStartTime() const
{
	QDateTime retVal = QDateTime::currentDateTime();

	retVal.addMSecs(quint64(GetElapsed() * 1000));

	return retVal;
}


double CPerformanceTimeStamp::GetElapsed() const
{
	Q_ASSERT(sizeof(qint64) == sizeof(LARGE_INTEGER));

	qint64 endCounter;
	::QueryPerformanceCounter((LARGE_INTEGER*)&endCounter);

	return double(endCounter - m_startCounter) / s_timerFrequence;
}


double CPerformanceTimeStamp::GetTimeTo(const istd::ITimeStamp& timeStamp) const
{
	const CPerformanceTimeStamp* natTimeStampPtr = dynamic_cast<const CPerformanceTimeStamp*>(&timeStamp);
	if (natTimeStampPtr != NULL){
		return GetTimeTo(*natTimeStampPtr);
	}
	else{
		return GetElapsed() - timeStamp.GetElapsed();
	}
}


void CPerformanceTimeStamp::WaitTo(double time) const
{
	int restMs;
	while ((restMs = int((time - GetElapsed()) * 1000)) > 0){
		::SleepEx(quint32(restMs), FALSE);
	}
}


double CPerformanceTimeStamp::GetTimerResolution() const
{
	return 1.0 / double(s_timerFrequence);
}


// reimplemented (istd::IChangeable)

int CPerformanceTimeStamp::GetSupportedOperations() const
{
	return SO_COPY;
}


bool CPerformanceTimeStamp::CopyFrom(const istd::IChangeable& object, CompatibilityMode /*mode*/)
{
	const CPerformanceTimeStamp* nativeTimerPtr = dynamic_cast<const CPerformanceTimeStamp*>(&object);
	if (nativeTimerPtr != NULL){
		SetNativeRepresentation(nativeTimerPtr->GetNativeRepresentation());

		return true;
	}
	else{
		const istd::ITimeStamp* timerPtr = dynamic_cast<const istd::ITimeStamp*>(&object);
		if (timerPtr != NULL){
			Start(timerPtr->GetElapsed());

			return true;
		}
	}

	return false;
}


// static attributes

qint64 CPerformanceTimeStamp::s_timerFrequence;
bool CPerformanceTimeStamp::s_isTimerFrequenceValid = (::QueryPerformanceFrequency((LARGE_INTEGER*)&CPerformanceTimeStamp::s_timerFrequence) != 0);


} // namespace iwin

