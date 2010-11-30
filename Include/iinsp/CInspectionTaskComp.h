#ifndef iinsp_CInspectionTaskComp_included
#define iinsp_CInspectionTaskComp_included


// ACF includes
#include "ibase/TLoggerCompWrap.h"

#include "iinsp/IInspectionTask.h"


namespace iinsp
{


/**
	Standard component implementation of interface iinsp::IInspectionTask.
*/
class CInspectionTaskComp:
			public ibase::CLoggerComponentBase,
			virtual public IInspectionTask
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_BAD_PARAMS_COUNT = 0x5af30,
		MI_NO_SUBTASK
	};

	I_BEGIN_COMPONENT(CInspectionTaskComp);
		I_REGISTER_INTERFACE(IInspectionTask);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN_MULTI_0(m_subtasksCompPtr, "Subtasks", "List of subtasks (suppliers)", true);
		I_ASSIGN_MULTI_0(m_toSerializeCompPtr, "ToSerialize", "Optional list of object should be serialized", false);
	I_END_COMPONENT;

	// reimplemented (iinsp::IInspectionTask)
	virtual int GetSubtasksCount() const;
	virtual iproc::ISupplier* GetSubtask(int subtaskIndex) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	I_MULTIREF(iproc::ISupplier, m_subtasksCompPtr);
	I_MULTIREF(iser::ISerializable, m_toSerializeCompPtr);
};


} // namespace iinsp


#endif // !iinsp_CInspectionTaskComp_included


