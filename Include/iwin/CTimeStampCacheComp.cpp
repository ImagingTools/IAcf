#include "iwin/CTimeStampCacheComp.h"


namespace iwin
{


// reimplemented (iinsp::ITimeStampProvider)

const istd::ITimeStamp* CTimeStampCacheComp::GetCurrentTimeStamp() const
{
	if (m_isTimerValid){
		return &m_timer;
	}

	return NULL;
}


// reimplemented (istd::IChangeable)

bool CTimeStampCacheComp::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const iinsp::ITimeStampProvider* providerPtr = dynamic_cast<const iinsp::ITimeStampProvider*>(&object);
	if (providerPtr != NULL){
		const istd::ITimeStamp* timerPtr = providerPtr->GetCurrentTimeStamp();
		if (timerPtr != NULL){
			m_isTimerValid = m_timer.CopyFrom(*timerPtr);
		}
		else{
			m_isTimerValid = false;
		}
		return true;
	}

	return false;
}


} // namespace iwin


