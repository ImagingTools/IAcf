#ifndef ilolv_CMultiTracerMessages_included
#define ilolv_CMultiTracerMessages_included


#include "ilolv/CTracerMessages.h"
#include "ilolv/CIoCardTracerMessages.h"


namespace ilolv
{


class CMultiTracerMessages
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

		typedef CTracerMessages::SetParams::Result Result;

		I_DWORD lineIndex;

		CTracerMessages::SetParams line;
	};

	struct SetLineIoParams
	{
		enum
		{
			Id = SetLineParams::Id + 1
		};

		typedef CTracerMessages::SetParams::Result Result;

		I_DWORD lineIndex;

		I_DWORD readyBitIndex;

		CIoCardTracerMessages::SetIoParams line;
	};

	struct SetUnitParams
	{
		enum
		{
			Id = SetLineIoParams::Id + 1
		};

		typedef CTracerMessages::SetUnitParams::Result Result;

		I_DWORD lineIndex;

		CTracerMessages::SetUnitParams line;
	};

	struct SetEjectorParams
	{
		enum
		{
			Id = SetUnitParams::Id + 1
		};

		typedef CTracerMessages::SetEjectorParams::Result Result;

		I_DWORD lineIndex;

		CTracerMessages::SetEjectorParams line;
	};

	struct SetMode
	{
		enum
		{
			Id = SetEjectorParams::Id + 1
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

		typedef CTracerMessages::SingleTrigger::Result Result;

		I_DWORD lineIndex;

		CTracerMessages::SingleTrigger line;
	};

	struct GetLineInfo
	{
		enum
		{
			Id = SingleTrigger::Id + 1
		};

		typedef CTracerMessages::GetLineInfo::Result Result;

		I_DWORD lineIndex;

		CTracerMessages::GetLineInfo line;
	};

	struct GetLineLightBarrierInfo
	{
		enum
		{
			Id = GetLineInfo::Id + 1
		};

		typedef CIoCardTracerMessages::GetLightBarrierInfo::Result Result;

		I_DWORD lineIndex;

		CIoCardTracerMessages::GetLightBarrierInfo line;
	};

	struct PopId
	{
		enum
		{
			Id = GetLineLightBarrierInfo::Id + 1
		};

		typedef CTracerMessages::PopId::Result Result;

		I_DWORD lineIndex;

		CTracerMessages::PopId line;
	};

	struct SetResult
	{
		enum
		{
			Id = PopId::Id + 1
		};

		typedef CTracerMessages::SetResult::Result Result;

		I_DWORD lineIndex;

		CTracerMessages::SetResult line;
	};
};


} // namespace ilolv


#endif // !ilolv_CMultiTracerMessages_included


