#ifndef imebase_CChannelAddressParamComp_included
#define imebase_CChannelAddressParamComp_included


// ACF includes
#include <icomp/CComponentBase.h>

// IACF includes
#include <MePck/IMeilhausAccess.h>
#include <MePck/CChannelSelectionNode.h>


namespace imebase
{


class CChannelAddressParamComp: public icomp::CComponentBase, public CChannelSelectionNode
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CChannelAddressParamComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(iprm::ISelectionParam);
		I_ASSIGN(m_mailhausAccessCompPtr, "MeilhausAccess", "Allows to read set of possible options based on connected Me hardware", true, "MeilhausAccess");
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(IMeilhausAccess, m_mailhausAccessCompPtr);
};

	
} // namespace imebase


#endif // !imebase_CChannelAddressParamComp_included


