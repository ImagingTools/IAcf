#ifndef ilolv_CSignalBitsMessages_included
#define ilolv_CSignalBitsMessages_included


#include "ilolv/ilolv.h"


namespace ilolv
{


class CSignalBitsMessages
{
public:
	enum SignalBit
	{
		SB_HEARTBEAT,
		SB_ERROR,
		SB_WARNING
	};

	struct SignalParams
	{
		/**
			Heartbeat signal period in microseconds.
		*/
		int heartbeatPeriod;
		int signalBitsCount;
	};

	struct SetParams: public SignalParams
	{
		enum
		{
			Id = 200
		};

		typedef void Result;
	};

	struct SetSignalBitIndex
	{
		enum
		{
			Id = SetParams::Id + 1
		};

		typedef void Result;

		int signalIndex;

		int bitIndex;
	};

	struct SetApplicationStatus
	{
		enum
		{
			Id = SetSignalBitIndex::Id + 1
		};

		typedef void Result;

		int status;
	};
};


} // namespace ilolv


#endif // !ilolv_CSignalBitsMessages_included


