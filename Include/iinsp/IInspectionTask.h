#ifndef iinsp_IInspectionTask_included
#define iinsp_IInspectionTask_included


// ACF includes
#include "iser/ISerializable.h"
#include "iproc/ISupplier.h"


namespace iinsp
{


/**
	Representation of complete inspection task.
	Inspection contains of set of subtask suppliers.
*/
class IInspectionTask: virtual public iser::ISerializable
{
public:
	virtual int GetSubtasksCount() const = 0;
	virtual iproc::ISupplier* GetSubtask(int subtaskIndex) const = 0;
};


} // namespace iinsp


#endif // !iinsp_IInspectionTask_included


