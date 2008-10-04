#ifndef ilolv_CGeneralInfoDriverBase_included
#define ilolv_CGeneralInfoDriverBase_included


#include "ilolv/IDriver.h"
#include "ilolv/CGeneralInfoCommands.h"


namespace ilolv
{


/**
	Implementation of general information (for example error handling) for driver.
*/
class CGeneralInfoDriverBase: virtual public IDriver
{
public:
	CGeneralInfoDriverBase();

	const CGeneralInfoCommands::GeneralInfoParams& GetGeneralInfoParams() const;

	// reimplemented (ilolv::IDriver)
	virtual bool OnCommand(
				I_DWORD commandCode,
				const void* commandBuffer,
				int commandBufferSize,
				void* responseBuffer,
				int responseBufferSize,
				I_DWORD& responseSize);
	virtual void OnHardwareInterrupt(I_DWORD interruptFlags);

protected:
	// reimplemented (ilolv::IDriver)
	virtual void AppendMessage(int category, int id, const char* text, bool doSend = true);

private:
	void OnPopMessageCommand(CGeneralInfoCommands::PopMessage::Result& result);
	void OnKeepAliveCommand();

	mutable CGeneralInfoCommands::PopMessage::Result m_message;
	int m_messagePosition;

	CGeneralInfoCommands::GeneralInfoParams m_params;

	__int64 m_nextMinKeepAliveTime;
};


// inline methods

inline const CGeneralInfoCommands::GeneralInfoParams& CGeneralInfoDriverBase::GetGeneralInfoParams() const
{
	return m_params;
}


} // namespace ilolv


#endif // !ilolv_CGeneralInfoDriverBase_included


