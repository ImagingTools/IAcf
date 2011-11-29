#include "iwin/CTimeStampCacheComp.h"


// ACF includes
#include "icomp/CComponentBase.h"

// ACF-Solutions includes
#include "iinsp/ITimeStampProvider.h"


namespace iwin
{


// reimplemented (iinsp::ITimeStampProvider)

const isys::ITimer* CTimeStampCacheComp::GetCurrentTimeStamp() const
{
	if (m_isTimerValid){
		return &m_timer;
	}

	return NULL;
}


// reimplemented (istd::IChangeable)

bool CTimeStampCacheComp::CopyFrom(const IChangeable& object)
{
	const iinsp::ITimeStampProvider* providerPtr = dynamic_cast<const iinsp::ITimeStampProvider*>(&object);
	if (providerPtr != NULL){
		const isys::ITimer* timerPtr = providerPtr->GetCurrentTimeStamp();
		if (timerPtr != NULL){
			const CTimer* nativeTimerPtr = dynamic_cast<const CTimer*>(timerPtr);
			if (nativeTimerPtr != NULL){
				m_timer.SetNativeRepresentation(nativeTimerPtr->GetNativeRepresentation());
			}
			else{
				m_timer.SetElapsed(timerPtr->GetElapsed());
			}
		}
		else{
			m_isTimerValid = false;
		}
		return true;
	}

	return false;
}


} // namespace iwin


