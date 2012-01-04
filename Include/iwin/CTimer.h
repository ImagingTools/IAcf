#ifndef iwin_CTimer_included
#define iwin_CTimer_included


#include "istd/istd.h"

#include "isys/ITimer.h"


namespace iwin
{


/**
	An timer implementation for the Windows plattform

	\ingroup Windows
*/
class CTimer: virtual public isys::ITimer
{
public:
	CTimer();

	bool IsVaild() const;

	double GetTimeTo(const CTimer& timer) const;

	I_QWORD GetNativeRepresentation() const;
	void SetNativeRepresentation(I_QWORD value);

	// reimplemented (isys::ITimer)
	virtual void Start(double elapsedTime = 0);
	virtual double GetElapsed() const;
	virtual double GetTimeTo(const ITimer& timer) const;
	virtual void WaitTo(double time) const;
	virtual double GetTimerResolution() const;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const istd::IChangeable& object);

private:
	I_SQWORD m_startCounter;

	// static attributes
	static I_SQWORD s_timerFrequence;
	static bool s_isTimerFrequenceValid;
};


// inline methods

} // namespace iwin


#endif // !iwin_CTimer_included
