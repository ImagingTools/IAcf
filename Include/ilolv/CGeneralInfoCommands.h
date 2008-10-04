#ifndef ilolv_CGeneralInfoCommands_included
#define ilolv_CGeneralInfoCommands_included


#include "ilolv/ilolv.h"


namespace ilolv
{


class CGeneralInfoCommands
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
		MAX_ERROR_MESSAGE_SIZE = 256
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
			int category;	// value of qstd::IResult::Category, -1 if no message
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
			char text[MAX_ERROR_MESSAGE_SIZE];
		};
	};
};


} // namespace ilolv


#endif // !ilolv_CGeneralInfoCommands_included


