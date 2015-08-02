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
	explicit CPerformanceTimeStamp(quint64 nativeValue);

	bool IsVaild() const;

	double GetTimeTo(const CPerformanceTimeStamp& timeStamp) const;

	quint64 GetNativeRepresentation() const;
	void SetNativeRepresentation(quint64 value);

	// reimplemented (istd::ITimeStamp)
	virtual void Start(double elapsedTime = 0);
	virtual QDateTime GetStartTime() const;
	virtual double GetElapsed() const;
	virtual double GetTimeTo(const istd::ITimeStamp& timeStamp) const;
	virtual void WaitTo(double time) const;
	virtual double GetTimerResolution() const;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

private:
	qint64 m_startCounter;

	// static attributes
	static qint64 s_timerFrequence;
	static bool s_isTimerFrequenceValid;
};


// inline methods

} // namespace iwin


#endif // !iwin_CPerformanceTimeStamp_included
