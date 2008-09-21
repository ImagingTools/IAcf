#ifndef ilolv_CGeneralInfoMessages_included
#define ilolv_CGeneralInfoMessages_included


#include "ilolv/ilolv.h"


namespace ilolv
{


class CGeneralInfoMessages
{
public:
	/**
		Categories of message info.
	 */
	enum MessageCategory
	{
		MC_INFO,
		MC_WARNING,
		MC_ERROR,
		MC_CRITICAL
	};

	enum MessageId
	{
		/**
			There was no response from application.
		*/
		MI_NO_RESPONSE = 500,
		/**
			Some internal error.
		*/
		MI_INTERNAL_ERROR
	};

	enum
	{
		MAX_ERROR_MESSAGE_SIZE = 256,
		MAX_MESSAGE_PARAMS_COUNT = 16
	};

	struct GeneralInfoParams
	{
		/**
			Maximal time span between two 'Keep Alive' messages in microseconds.
		*/
		int maxKeepAliveTime;
	};


	struct SetParams: public GeneralInfoParams
	{
		enum
		{
			Id = 100
		};

		typedef void Result;
	};

	/**
		Keep Alive message informing driver that application is working.
	*/
	struct KeepAlive
	{
		enum
		{
			Id = SetParams::Id + 1
		};

		typedef void Result;
	};

	/**
		Get driver message and remove it from message list.
	*/
	struct PopMessage
	{
		enum
		{
			Id = KeepAlive::Id + 1
		};

		struct Result
		{
			/**
				Message category \sa MessageCategory.
			*/
			I_SDWORD category;	// value of qstd::IResult::Category, -1 if no message
			/**
				Unique message Id used to automatical message processing.
			*/
			I_DWORD id;
			/**
				Message flags.
			*/
			I_DWORD flags;
			/**
				Number of parameters.
			*/
			I_SDWORD paramsCount;
			/**
				Human readable message text.
			*/
			I_BYTE message[MAX_ERROR_MESSAGE_SIZE];
			/**
				List of parameter (as number).
			*/
			I_SDWORD params[MAX_MESSAGE_PARAMS_COUNT];
		};
	};
};


} // namespace ilolv


#endif // !ilolv_CGeneralInfoMessages_included


