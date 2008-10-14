#ifndef ilolv_CMultiTracerCommands_included
#define ilolv_CMultiTracerCommands_included


#include "ilolv/CTracerCommands.h"
#include "ilolv/CIoCardTracerCommands.h"


namespace ilolv
{


class CMultiTracerCommands
{
public:
	struct MultiTracerParams
	{
		int linesCount;
	};

	// commands
	struct SetParams: public MultiTracerParams
	{
		enum
		{
			Id = 600
		};

		typedef void Result;
	};

	struct SetLineParams
	{
		enum
		{
			Id = SetParams::Id + 1
		};

		typedef CTracerCommands::SetParams::Result Result;

		I_DWORD lineIndex;

		CTracerCommands::SetParams line;
	};

	struct SetLineIoParams
	{
		enum
		{
			Id = SetLineParams::Id + 1
		};

		typedef CTracerCommands::SetParams::Result Result;

		I_DWORD lineIndex;

		CIoCardTracerCommands::SetIoParams line;
	};

	struct SetUnitParams
	{
		enum
		{
			Id = SetLineIoParams::Id + 1
		};

		typedef CTracerCommands::SetUnitParams::Result Result;

		I_DWORD lineIndex;

		CTracerCommands::SetUnitParams line;
	};

	struct SetEjectorParams
	{
		enum
		{
			Id = SetUnitParams::Id + 1
		};

		typedef CTracerCommands::SetEjectorParams::Result Result;

		I_DWORD lineIndex;

		CTracerCommands::SetEjectorParams line;
	};

	/**
		Command for setting parameter of light barrier for specified line.
	*/
	struct SetLightBarrierParams
	{
		enum
		{
			Id = SetEjectorParams::Id + 1
		};

		typedef CTracerCommands::SetLightBarrierParams::Result Result;

		I_DWORD lineIndex;

		CTracerCommands::SetLightBarrierParams line;
	};

	struct SetMode
	{
		enum
		{
			Id = SetLightBarrierParams::Id + 1
		};

		typedef void Result;

		I_DWORD mode;
	};

	struct SingleTrigger
	{
		enum
		{
			Id = SetMode::Id + 1
		};

		typedef CTracerCommands::SingleTrigger::Result Result;

		I_DWORD lineIndex;

		CTracerCommands::SingleTrigger line;
	};

	struct GetLineInfo
	{
		enum
		{
			Id = SingleTrigger::Id + 1
		};

		typedef CTracerCommands::GetLineInfo::Result Result;

		I_DWORD lineIndex;

		CTracerCommands::GetLineInfo line;
	};

	struct GetLineLightBarrierInfo
	{
		enum
		{
			Id = GetLineInfo::Id + 1
		};

		typedef CIoCardTracerCommands::GetLightBarrierInfo::Result Result;

		I_DWORD lineIndex;

		CIoCardTracerCommands::GetLightBarrierInfo line;
	};

	struct PopId
	{
		enum
		{
			Id = GetLineLightBarrierInfo::Id + 1
		};

		typedef CTracerCommands::PopId::Result Result;

		I_DWORD lineIndex;

		CTracerCommands::PopId line;
	};

	struct SetResult
	{
		enum
		{
			Id = PopId::Id + 1
		};

		typedef CTracerCommands::SetResult::Result Result;

		I_DWORD lineIndex;

		CTracerCommands::SetResult line;
	};
};


} // namespace ilolv


#endif // !ilolv_CMultiTracerCommands_included


