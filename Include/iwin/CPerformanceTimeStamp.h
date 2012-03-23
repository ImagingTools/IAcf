#ifndef iwin_CPerformanceTimeStamp_included
#define iwin_CPerformanceTimeStamp_included


// ACF includes
#include "istd/ITimeStamp.h"


namespace iwin
{


/**
	High perfomance time stamp implementation.

	\ingroup Windows
*/
class CPerformanceTimeStamp: virtual public istd::ITimeStamp
{
public:
	CPerformanceTimeStamp();

	bool IsVaild() const;

	double GetTimeTo(const CPerformanceTimeStamp& timeStamp) const;

	I_QWORD GetNativeRepresentation() const;
	void SetNativeRepresentation(I_QWORD value);

	// reimplemented (istd::ITimeStamp)
	virtual void Start(double elapsedTime = 0);
	virtual double GetElapsed() const;
	virtual double GetTimeTo(const istd::ITimeStamp& timeStamp) const;
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


#endif // !iwin_CPerformanceTimeStamp_included
