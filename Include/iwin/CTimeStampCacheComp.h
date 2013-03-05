#ifndef iwin_CTimeStampCacheComp_included
#define iwin_CTimeStampCacheComp_included


// ACF includes
#include "icomp/CComponentBase.h"

// ACF-Solutions includes
#include "iinsp/ITimeStampProvider.h"

// IACF includes
#include "iwin/CPerformanceTimeStamp.h"


namespace iwin
{


/**
	Time stamp provider returning always stored time stamp.
	Stored time stamp can be changed only copying their from another time stamp provider using CopyFrom method.
	This object will be used to manage threading barrier for object supplier chain.
*/
class CTimeStampCacheComp:
			public icomp::CComponentBase,
			virtual public iinsp::ITimeStampProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTimeStampCacheComp);
		I_REGISTER_INTERFACE(iinsp::ITimeStampProvider);
	I_END_COMPONENT;

	// reimplemented (iinsp::ITimeStampProvider)
	virtual const istd::ITimeStamp* GetCurrentTimeStamp() const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

private:
	bool m_isTimerValid;
	iwin::CPerformanceTimeStamp m_timer;
};


} // namespace iwin


#endif // !iwin_CTimeStampCacheComp_included


