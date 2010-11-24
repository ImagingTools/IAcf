#include "CChannelAddressParamComp.h"


namespace imebase
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CChannelAddressParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_mailhausAccessCompPtr.IsValid()){
		m_mailhausAccessCompPtr->CreateSelectionTree(*this);
	}
}

	
void CChannelAddressParamComp::OnComponentDestroyed()
{
	ResetNodes();

	BaseClass::OnComponentDestroyed();
}


} // namespace imebase


