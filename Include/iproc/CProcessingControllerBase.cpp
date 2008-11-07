#include "iproc/CProcessingControllerBase.h"
#include "iproc/COperatorBase.h"


namespace iproc
{


// public methods

// reimplemented (iproc::IProcessingController)

void CProcessingControllerBase::AddProgressHandler(iproc::IProgressEventHandler* progressHandlerPtr)
{
	m_progressHandlers.insert(progressHandlerPtr);
}


// protected methods

// reimplemented (imod::CSingleModelObserverBase)

void CProcessingControllerBase::OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	bool doUpdate = ((updateFlags & iproc::IOperator::ProgressChanged) != 0);
	if (doUpdate){
		iproc::IOperator* operatorPtr = GetObjectPtr();
		I_ASSERT(operatorPtr != NULL);
	
		double currentProgress = operatorPtr->GetProgress();

		for (ProgressHandlers::iterator index = m_progressHandlers.begin(); index != m_progressHandlers.end(); index++){
			(*index)->OnProgress(currentProgress);
		}
	}
}


} // namespace iproc


