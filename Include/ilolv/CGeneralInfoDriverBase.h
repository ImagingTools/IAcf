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

	// reimplemented (ilolv::IDriver)
	virtual bool OnInstruction(
				I_DWORD instructionCode,
				const void* instructionBuffer,
				int instructionBufferSize,
				void* responseBuffer,
				int responseBufferSize,
				I_DWORD& responseSize);
	virtual void OnHardwareInterrupt(I_DWORD interruptFlags);
	virtual void OnPulse();

protected:
	struct Message
	{
		/**
			Message category \sa MessageCategory.
		*/
		int category;
		/**
			Unique message Id used to automatical message processing.
		*/
		int id;
		/**
			Message flags.
		*/
		int flags;
		/**
			Number of parameters.
		*/
		int paramsCount;
		/**
			Human readable message text.
		*/
		char text[CGeneralInfoMessages::MAX_ERROR_MESSAGE_SIZE];
		/**
			List of parameter (as number).
		*/
		int params[CGeneralInfoMessages::MAX_MESSAGE_PARAMS_COUNT];
	};

	/**	Set message text.
	 */
	void SendMessage(int category, int id, const char* errorTxt, int* valuesPtr = NULL, int paramsCount = 0);

private:
	void OnPopMessageInstruction(CGeneralInfoMessages::PopMessage::Result& result);
	void OnAliveSignal();

	mutable Message m_message;

	CGeneralInfoMessages::GeneralInfoParams m_params;

	__int64 m_nextMinKeepAliveTime;
};


} // namespace ilolv


#endif // !ilolv_CDriverMessageProviderBase_included


