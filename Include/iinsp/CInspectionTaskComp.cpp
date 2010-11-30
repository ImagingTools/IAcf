#include "iinsp/CInspectionTaskComp.h"


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iinsp
{


// reimplemented (iinsp::IInspectionTask)

int CInspectionTaskComp::GetSubtasksCount() const
{
	return m_subtasksCompPtr.GetCount();
}


iproc::ISupplier* CInspectionTaskComp::GetSubtask(int subtaskIndex) const
{
	return m_subtasksCompPtr[subtaskIndex];
}


// reimplemented (iser::ISerializable)

bool CInspectionTaskComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag taskListTag("SubtaskList", "List of inspection subtasks");
	static iser::CArchiveTag taskTag("Subtask", "Single subtask");

	bool retVal = true;

	if (m_toSerializeCompPtr.IsValid()){
		int subtasksCount = m_toSerializeCompPtr.GetCount();

		retVal = retVal && archive.BeginMultiTag(taskListTag, taskTag, subtasksCount);

		if (!retVal || (!archive.IsStoring() && subtasksCount != m_toSerializeCompPtr.GetCount())){
			return false;
		}

		for (int i = 0; i < subtasksCount; ++i){
			retVal = retVal && archive.BeginTag(taskTag);

			iser::ISerializable* serializablePtr = m_toSerializeCompPtr[i];
			if (serializablePtr != NULL){
				SendWarningMessage(MI_BAD_PARAMS_COUNT, "Bad number of parameter to serialize");
				retVal = retVal && serializablePtr->Serialize(archive);
			}

			retVal = retVal && archive.EndTag(taskTag);
		}
	}
	else{
		int subtasksCount = m_subtasksCompPtr.GetCount();

		retVal = retVal && archive.BeginMultiTag(taskListTag, taskTag, subtasksCount);

		if (!retVal || (!archive.IsStoring() && subtasksCount != m_subtasksCompPtr.GetCount())){
			SendWarningMessage(MI_BAD_PARAMS_COUNT, "Bad number of parameter to serialize");
			return false;
		}

		for (int i = 0; i < subtasksCount; ++i){
			retVal = retVal && archive.BeginTag(taskTag);

			iproc::ISupplier* taskPtr = m_subtasksCompPtr[i];
			if (taskPtr == NULL){
				SendCriticalMessage(MI_NO_SUBTASK, "No subtask connected");
				return false;
			}

			iprm::IParamsSet* paramsSetPtr = taskPtr->GetModelParametersSet();
			if (paramsSetPtr != NULL){
				retVal = retVal && paramsSetPtr->Serialize(archive);
			}

			retVal = retVal && archive.EndTag(taskTag);
		}
	}

	retVal = retVal && archive.EndTag(taskListTag);

	return retVal;
}


} // namespace iinsp


