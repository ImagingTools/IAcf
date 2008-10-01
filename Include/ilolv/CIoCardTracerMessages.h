#ifndef ilolv_CIoCardTracerMessages_included
#define ilolv_CIoCardTracerMessages_included


#include "ilolv/CTracerMessages.h"


namespace ilolv
{


class CIoCardTracerMessages
{
public:
	struct IoParams
	{
		int lightBarriersBitIndex;
		int ejectorsBitIndex;
		int triggersBitIndex;
		int iosBitIndex;
	};

	// commands
	struct SetIoParams: IoParams
	{
		enum
		{
			Id = 310
		};

		typedef CTracerMessages::SetParams::Result Result;
	};

	struct GetLightBarrierInfo
	{
		enum
		{
			Id = SetIoParams::Id + 1
		};

		struct Result
		{
			bool state;
		};

		int lightBarrierIndex;
	};
};


} // namespace ilolv


#endif // !ilolv_CIoCardTracerMessages_included


