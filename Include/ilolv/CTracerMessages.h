#ifndef ilolv_CTracerMessages_included
#define ilolv_CTracerMessages_included


#include "ilolv/CInspectionUnitMessages.h"


namespace ilolv
{


class CTracerMessages
{
public:
	enum MessageId
	{
		/**
			Some problem in object queue.
		*/
		MI_QUEUE_ERROR = 13430,
		/**
			Object should be ejected, but was found during ejection control.
		*/
		MI_OBJECT_UNEJECTED,
		/**
			Object should not be ejected, but was found during ejection control.
		*/
		MI_OBJECT_EJECTED,
	};

	struct EjectorParams
	{
		/**
			Flag saying this exector is enabled.
		*/
		bool isEnabled;

		/**
			Time ejector is on.
			This time is in microseconds.
		*/
		int maxEjectorOnTime;

		/**
			Number of encoder clocks between ejector and imaginated base position.
		*/
		int ejectorOffset;

		/**
			Distance ejector is on.
		*/
		int ejectorOnDistance;
	};

	struct TracerParams
	{
		enum
		{
			Id = 390
		};

		typedef void Result;

		int unitsCount;
		int ejectorsCount;
		int autonomeEjectorIndex;
		int minObjectsDistance;
		int positionTolerance;
		int ioBitDuration;
		bool isEjectionControlEnabled;
		int ejectionControlOffset;
	};

	struct SetParams: public TracerParams
	{
		enum
		{
			Id = 390
		};

		typedef void Result;
	};

	struct SetUnitParams
	{
		enum
		{
			Id = SetParams::Id + 1
		};

		typedef void Result;

		int unitIndex;

		CInspectionUnitMessages::SetParams unit;
	};

	struct SetEjectorParams
	{
		enum
		{
			Id = SetUnitParams::Id + 1
		};

		typedef void Result;

		int ejectorIndex;

		EjectorParams ejector;
	};

	struct SetMode: public CInspectionUnitMessages::SetMode
	{
		enum
		{
			Id = SetEjectorParams::Id + 1
		};
	};

	struct SingleTrigger
	{
		enum
		{
			Id = SetMode::Id + 1
		};

		struct Result
		{
			IDriver::NativeTimer nativeTimestamp;
		};

		int unitIndex;

		CInspectionUnitMessages::SingleTrigger unit;
	};

	struct GetLineInfo
	{
		enum
		{
			Id = SingleTrigger::Id + 1
		};

		struct Result
		{
			I_DWORD linePos;
			I_DWORD lastDetectedObjectIndex;	// inclusive objects when automatic off
		};
	};

	struct PopId
	{
		enum
		{
			Id = GetLineInfo::Id + 1
		};

		typedef CInspectionUnitMessages::PopId::Result Result;

		int unitIndex;
	};

	struct SetResult
	{
		enum
		{
			Id = PopId::Id + 1
		};

		typedef CInspectionUnitMessages::SetResult::Result Result;

		int unitIndex;

		CInspectionUnitMessages::SetResult unit;
	};
};


} // namespace ilolv


#endif // !ilolv_CTracerMessages_included


