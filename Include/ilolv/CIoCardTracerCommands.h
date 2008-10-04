#ifndef ilolv_CIoCardTracerCommands_included
#define ilolv_CIoCardTracerCommands_included


#include "ilolv/CTracerCommands.h"


namespace ilolv
{


class CIoCardTracerCommands
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

		typedef CTracerCommands::SetParams::Result Result;
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


#endif // !ilolv_CIoCardTracerCommands_included


