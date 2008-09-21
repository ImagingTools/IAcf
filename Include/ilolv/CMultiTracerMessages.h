#ifndef ilolv_CMultiTracerMessages_included
#define ilolv_CMultiTracerMessages_included


#include "ilolv/CTracerMessages.h"


namespace ilolv
{


class CMultiTracerMessages
{
public:
	struct MultiTracerParams
	{
		int linesCount;
	};

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

		I_DWORD lightBarriersBitIndex;
		I_DWORD ejectionControlBitIndex;
		I_DWORD ejectorsBitIndex;
		I_DWORD triggersBitIndex;
		I_DWORD readyBitIndex;
		I_DWORD iosBitIndex;

		CTracerMessages::SetParams line;
	};

	struct SetUnitParams
	{
		enum
		{
			Id = SetLineParams::Id + 1
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

		struct Result: public CTracerMessages::GetLineInfo::Result
		{
			I_DWORD sensorBits;
			I_BYTE ejectionControlBit;
		};

		I_DWORD lineIndex;
	};

	struct PopId
	{
		enum
		{
			Id = GetLineInfo::Id + 1
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


