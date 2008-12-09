#include "iproc/COperatorBase.h"


#include "istd/CStaticServicesProvider.h"

#include "iser/IArchive.h"

#include "ibase/CMessage.h"

#include "isys/CSectionBlocker.h"


namespace iproc
{


COperatorBase::COperatorBase()
:	m_progress(0.0),
	m_state(StateUnknown)
{
	m_mutex = istd::GetService<isys::ICriticalSection>();

	I_ASSERT(m_mutex != NULL);
}


bool COperatorBase::IsAborted() const
{
	return GetProcessingState() == StateAborted;
}


void COperatorBase::SetProcessingState(int processingState)
{
	isys::CSectionBlocker lock(m_mutex);

	m_state = (StateInfo)processingState;
}


// reimplemented (iproc::IOperator)

IOperator::StateInfo COperatorBase::GetProcessingState() const
{
	IOperator::StateInfo state = StateUnknown;

	isys::CSectionBlocker lock(m_mutex);

	state = m_state;

	return m_state;
}


bool COperatorBase::BeforeProcessing()
{
	SetProcessingState(StateUnknown);

	return true;
}


bool COperatorBase::AfterProcessing()
{
	SetProcessingState(StateFinished);

	return true;
}

void COperatorBase::Cancel()
{
	SetProcessingState(StateAborted);
}


// reimplemented (iproc::IProgressInfo)

double COperatorBase::GetProgress() const
{
	return m_progress;
}


// protected methods

// reimplemented (ibase::TLoggerWrap)

bool COperatorBase::SendLogMessage(ibase::IMessage::MessageCategory category, int id, const istd::CString& message, const istd::CString& /*messageSource*/) const
{
	return BaseClass::SendLogMessage(category, id, message, GetName());
}


} // namespace iproc


