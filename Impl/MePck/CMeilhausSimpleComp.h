#ifndef imebase_CMeilhausSimpleComp_included
#define imebase_CMeilhausSimpleComp_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include "icomp/CComponentBase.h"
#include "iprm/ISelectionParam.h"
#include "iproc/IProcessor.h"

// IACF includes
#include "isig/ISamplingParams.h"
#include "isig/ISamplingConstraints.h"

#include "IMeilhausAccess.h"
#include "CMeContext.h"


namespace imebase
{


/**
	Implementation allowing to operate on ME4600 card's analog I/O.
*/
class CMeilhausSimpleComp:
			public icomp::CComponentBase,
			virtual public iproc::IProcessor,
			virtual public IMeilhausAccess
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CMeilhausSimpleComp);
		I_REGISTER_INTERFACE(iproc::IProcessor);
		I_REGISTER_INTERFACE(IMeilhausAccess);
		I_REGISTER_INTERFACE(isig::ISamplingConstraints);

		I_ASSIGN(m_isOutputAttrPtr, "IsOutput", "If true this processor operate in output mode, otherwise input mode", true, false);
		I_ASSIGN(m_addressParamIdAttrPtr, "AddressParamId", "ID of channel address in parameter set (iprm::ISelectionParam)", true, "ChannelAddress");
		I_ASSIGN(m_samplingParamsIdAttrPtr, "SamplingParamsId", "ID of sampling parameters in parameter set", true, "SamplingParams");
	I_END_COMPONENT;

	CMeilhausSimpleComp();

	// reimplemented (iproc::IProcessor)
	virtual int GetProcessorState(const iprm::IParamsSet* paramsPtr) const;
	virtual bool AreParamsAccepted(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				const istd::IChangeable* outputPtr) const;
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);
	virtual int BeginTask(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);
	virtual int WaitTaskFinished(
				int taskId = -1,
				double timeoutTime = -1,
				bool killOnTimeout = true);
	virtual void CancelTask(int taskId = -1);
	virtual int GetReadyTask();
	virtual int GetTaskState(int taskId = -1) const;
	virtual void InitProcessor(const iprm::IParamsSet* paramsPtr);

	// reimplemented (imebase::IMeilhausAccess)
	virtual bool CreateSelectionTree(CChannelSelectionNode& result) const;

	// reimplemented (isig::ISamplingConstraints)
	virtual istd::CRange GetIntervalRange() const;
	virtual bool IsSamplingModeSupported(int mode) const;
	virtual istd::CRange GetValueRange(
				bool forInput = true,
				bool forOutput = true,
				const iprm::IParamsSet* paramsSetPtr = NULL) const;
	virtual int GetMaximalSamplesCount(
				bool forInput = true,
				bool forOutput = true,
				const iprm::IParamsSet* paramsSetPtr = NULL) const;


protected:
	int PullNextTaskId();
	bool GetChannelAddress(const iprm::IParamsSet* paramsPtr, CMeAddr& result) const;
	const isig::ISamplingParams* GetSamplingParams(const iprm::IParamsSet* paramsPtr) const;

	virtual int WaitSingleTaskFinished(CMeContext& context, double timeoutTime, bool killOnTimeout);

private:
	I_ATTR(bool, m_isOutputAttrPtr);

	I_ATTR(QByteArray, m_addressParamIdAttrPtr);
	I_ATTR(QByteArray, m_samplingParamsIdAttrPtr);

	typedef QMap<int, CMeContext*> TasksList;
	TasksList m_activeTaskList;

	int m_lastTaskId;
};


} // namespace imebase


#endif // !imebase_CMeilhausSimpleComp_included

