#ifndef imebase_CMeContext_included
#define imebase_CMeContext_included


// Qt includes
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>

// ACF-Solutions includes
#include "imeas/IDataSequence.h"

#include "imebase.h"


namespace imebase
{


/**
	Context for data processing
*/
class CMeContext
{
public:
	CMeContext(const CMeAddr& address, bool isOutput, imeas::IDataSequence* containerPtr);
	~CMeContext();

	/**
		Register this context to MEiDS
	*/
	bool Register(double interval);
	void Unregister();

	int GetId()  const;

	/**
		Check buffer status.
	*/
	int GetCount()  const;
	bool IsDone();
	double GetInterval() const;

	/**
		Wait for task to end. If time out return false.
		\param	timeout	time out in seconds.
	*/
	bool Wait(double timeout);

	void CopyToContainer();
	void CopyFromContainer();

protected:
	bool ConfigInputStream();
	bool ConfigOutputStream();

	bool StartStream();

	// static methods
	static int __stdcall cbAIFunc(int device, int subdevice, int count, void* context, int error);
	static int __stdcall cbAOFunc(int device, int subdevice, int count, void* context, int error);

private:
	QMutex m_activeTaskMutex;
	QWaitCondition m_dataReadyCondition;

	CMeAddr m_address;
	int m_bufferCount;
	QVector<int> m_hwBuffer;
	bool m_isOutput;
	double m_interval;

	imeas::IDataSequence& m_samplesSequence;
};


} // namespace imebase


#endif // !imebase_CMeContext_included


