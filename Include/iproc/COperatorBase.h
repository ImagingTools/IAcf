#ifndef iproc_COperatorBase_included
#define iproc_COperatorBase_included



#include "iproc/IOperator.h"
#include "iproc/IProcessingEventHandler.h"

#include "isys/ICriticalSection.h"

#include "ibase/IMessageConsumer.h"
#include "ibase/THierarchicalBase.h"
#include "ibase/TEnableableWrap.h"
#include "ibase/TNamedWrap.h"
#include "ibase/TLoggableWrap.h"
#include "ibase/CMessage.h"


namespace iproc
{		


/**
	A base implementation of the operator interface.

	This is a common class for all operator classes that implement the operator interface. 
	It is advisable to use this implementation by programming of the own operators. 
*/

class COperatorBase: public ibase::TLoggableWrap<
								ibase::TEnableableWrap<
									ibase::THierarchicalBase<
										ibase::TNamedWrap<iproc::IOperator> > > >
{
public:

	typedef ibase::TLoggableWrap<
				ibase::TEnableableWrap<
					ibase::THierarchicalBase<
						ibase::TNamedWrap<iproc::IOperator> > > > BaseClass;

	COperatorBase();

	enum MessageId
	{
		MI_GENERAL = 0x1af2330
	};

	virtual bool IsAborted() const;
	virtual void SetProcessingState(int processingState);

	// reimplemented (iproc::IOperator)
	virtual StateInfo GetProcessingState() const;
	virtual bool BeforeProcessing();
	virtual bool AfterProcessing();
	virtual void Cancel();

	// reimplemented (iproc::IProgressInfo)
	virtual double GetProgress() const;

protected:
	// reimplemented (ibase::TLoggableWrap)
	virtual bool SendLogMessage(ibase::IMessage::MessageCategory category, int id, const istd::CString& message, const istd::CString& messageSource) const;

protected:
	double m_progress;

private:
	StateInfo m_state;

	mutable isys::ICriticalSection* m_mutex; 
};


} // namespace iproc


#endif // !iproc_COperatorBase_included


