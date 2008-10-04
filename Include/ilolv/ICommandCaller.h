#ifndef ilolv_ICommandCaller_included
#define ilolv_ICommandCaller_included


namespace ilolv
{


/**
	Call low level commands (for example implemented in hardware driver).
	This commands will call method ilolv::IDriver::OnCommand on driver implementation level.
*/
class ICommandCaller: public istd::IPolymorphic
{
public:
	/**
		Call driver command.
		\param	commandCode			code of command.
		\param	commandBuffer		pointer to command buffer.
		\param	commandBufferSize	size of command buffer in bytes.
		\param	responseBuffer		pointer to response buffer.
		\param	responseSize		size of response buffer in bytes.
									It is in/out value, should be set to real size of response.
		\return	true if this command was called correctly.
	*/
	virtual bool CallCommand(
				I_DWORD commandCode,
				const void* commandBuffer,
				int commandBufferSize,
				void* responseBuffer,
				int responseBufferSize,
				I_DWORD& responseSize) = 0;
};


} // namespace ilolv


#endif // !ilolv_ICommandCaller_included


