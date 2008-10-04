#ifndef ilolv_CTracerCommands_included
#define ilolv_CTracerCommands_included


#include "ilolv/CInspectionUnitCommands.h"


namespace ilolv
{


class CTracerCommands
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

	/**
		Working mode of object tracer.
	*/
	enum TracerMode
	{
		/**
			No action is supported. Whole actions of object tracer are disabled.
		*/
		TM_DISABLED,
		/**
			Manual mode. Some actions like manual trigger can be made.
		*/
		TM_MANUAL,
		/**
			Automatic mode.
			Objects will be traced, if there is no response from application they will be autonomic ejected.
		*/
		TM_AUTOMATIC,
		/**
			Diagnostic mode. Some special actions are possible.
		*/
		TM_DIAGNOSTIC
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
		int position;

		/**
			Distance ejector is on.
		*/
		int onDistance;
		/**
			Reaction delay of this ejector (in microseconds).
			To ensure correct behaviour, ejector signal will be send earlier according to this value.
		*/
		int reactionDelay;
	};

	struct TracerParams
	{
		enum
		{
			Id = 390
		};

		typedef void Result;

		/**
			Number of inspection units.
		*/
		int unitsCount;
		/**
			Number of ejectors.
		*/
		int ejectorsCount;
		/**
			Number of light barriers.
		*/
		int lightBarriersCount;
		/**
			Index of ejector used if no ejector was specified by application.
			This ejector will be used for not processed object.
			If it negative, no ejection will be done.
		*/
		int autonomeEjectorIndex;
		/**
			Minimal size of an object (in encoder ticks).
		*/
		int minObjectSize;
		/**
			Minimal distance between two objects (in encoder ticks) needed to treat it as two objects.
		*/
		int minObjectsDistance;
		/**
			Tolerance of light barrier position used to assign object in queue to the next light barriers.
		*/
		int positionTolerance;
		/**
			If true ejection control is enabled.
		*/
		bool isEcEnabled;
		/**
			Parameter of ejection control light barrier.
		*/
		CInspectionUnitCommands::LightBarrierParams ecLightBarrier;
	};

	// commands
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

		CInspectionUnitCommands::SetParams unit;
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

	struct SetMode: public CInspectionUnitCommands::SetMode
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
			/**
				If manual trigger could be done, it wil be set to true.
			*/
			bool isDone;
			/**
				Timestamp of manual trigger.
			*/
			IDriver::NativeTimer nativeTimestamp;
		};

		int unitIndex;

		CInspectionUnitCommands::SingleTrigger unit;
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

		typedef CInspectionUnitCommands::PopId::Result Result;

		int unitIndex;
	};

	struct SetResult
	{
		enum
		{
			Id = PopId::Id + 1
		};

		typedef CInspectionUnitCommands::SetResult::Result Result;

		int unitIndex;

		CInspectionUnitCommands::SetResult unit;
	};
};


} // namespace ilolv


#endif // !ilolv_CTracerCommands_included


