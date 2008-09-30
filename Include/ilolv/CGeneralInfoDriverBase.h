#ifndef ilolv_CDriverMessageProviderBase_included
#define ilolv_CDriverMessageProviderBase_included


#include "ilolv/IDriver.h"
#include "ilolv/CGeneralInfoMessages.h"


namespace ilolv
{


/**
	Base class for implementation of production driver on the driver side.
*/
class CGeneralInfoDriverBase: virtual public IDriver
{
public:
	CGeneralInfoDriverBase();

	const CGeneralInfoMessages::GeneralInfoParams& GetGeneralInfoParams() const;

	// reimplemented (ilolv::IDriver)
	virtual bool OnInstruction(
				I_DWORD instructionCode,
				const void* instructionBuffer,
				int instructionBufferSize,
				void* responseBuffer,
				int responseBufferSize,
				I_DWORD& responseSize);
	virtual void OnHardwareInterrupt(I_DWORD interruptFlags);
	virtual void OnPeriodicPulse();

protected:
	// reimplemented (ilolv::IDriver)
	virtual void AppendMessage(int category, int id, const char* text, bool doSend = true);

private:
	void OnPopMessageInstruction(CGeneralInfoMessages::PopMessage::Result& result);
	void OnKeepAlive();

	mutable CGeneralInfoMessages::PopMessage::Result m_message;
	int m_messagePosition;

	CGeneralInfoMessages::GeneralInfoParams m_params;

	__int64 m_nextMinKeepAliveTime;
};


// inline methods

inline const CGeneralInfoMessages::GeneralInfoParams& CGeneralInfoDriverBase::GetGeneralInfoParams() const
{
	return m_params;
}


} // namespace ilolv


#endif // !ilolv_CDriverMessageProviderBase_included


