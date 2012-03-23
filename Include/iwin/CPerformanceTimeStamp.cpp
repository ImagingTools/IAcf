#include "iwin/CPerformanceTimeStamp.h"


// Windows includes
#include <windows.h>

// ACF includes
#include "istd/TChangeNotifier.h"


namespace iwin
{


CPerformanceTimeStamp::CPerformanceTimeStamp()
{
	m_startCounter = 0;

	if (s_isTimerFrequenceValid){
		Start();
	}
}


bool CPerformanceTimeStamp::IsVaild() const
{
	return s_isTimerFrequenceValid;
}


double CPerformanceTimeStamp::GetTimeTo(const CPerformanceTimeStamp& timeStamp) const
{
	return (m_startCounter - timeStamp.m_startCounter) / double(s_timerFrequence);
}


I_QWORD CPerformanceTimeStamp::GetNativeRepresentation() const
{
	return I_QWORD(m_startCounter);
}


void CPerformanceTimeStamp::SetNativeRepresentation(I_QWORD value)
{
	istd::CChangeNotifier notifier(this);

	m_startCounter = I_SQWORD(value);
}


// reimplemented (istd::ITimeStamp)

void CPerformanceTimeStamp::Start(double elapsedTime)
{
	I_ASSERT(sizeof(I_SQWORD) == sizeof(LARGE_INTEGER));

	istd::CChangeNotifier notifier(this);

	LARGE_INTEGER currentCounter;
	::QueryPerformanceCounter(&currentCounter);

	m_startCounter = (I_SQWORD&)currentCounter - I_SQWORD(elapsedTime * s_timerFrequence);
}


double CPerformanceTimeStamp::GetElapsed() const
{
	I_ASSERT(sizeof(I_SQWORD) == sizeof(LARGE_INTEGER));

	I_SQWORD endCounter;
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
		::SleepEx(I_DWORD(restMs), FALSE);
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


bool CPerformanceTimeStamp::CopyFrom(const istd::IChangeable& object)
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

I_SQWORD CPerformanceTimeStamp::s_timerFrequence;
bool CPerformanceTimeStamp::s_isTimerFrequenceValid = (::QueryPerformanceFrequency((LARGE_INTEGER*)&CPerformanceTimeStamp::s_timerFrequence) != 0);


} // namespace iwin

