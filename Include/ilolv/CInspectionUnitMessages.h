#ifndef ilolv_CInspectionUnitMessages_included
#define ilolv_CInspectionUnitMessages_included


#include "ilolv/IDriver.h"


namespace ilolv
{


class CInspectionUnitMessages
{
public:
	enum MessageId
	{
		/**
			Unidentified object was found.
		*/
		MI_UNIDENTIFIED_OBJECT_FOUND,
	};

	struct LightBarrierParams
	{
		/**
			Index of used light barrier.
			If this value is negative, this function is disabled.
		*/
		int index;
		/**
			Number of encoder clocks between light barrier and imaginated base position.
		*/
		int offset;
	};

	struct UnitParams
	{
		enum
		{
			FALLING_EDGE = 0x100
		};
		LightBarrierParams lightBarrier;

		/**
			Position inside of edge when trigger should be done.
			If this value is 0, rising edge is mean, if this is equal to FALLING_EDGE falling edge.
			Other values mean some proportion between.
		*/
		int edgePosition;

		/**
			Number of microseconds should be trigger active.
		*/
		int triggerDuration;
		/**
			Relaxation time needed to send next trigger.
			This can usefull for example to block new camera trigger before complete acquision of previos image is done.
			Please note, that in this case object will be processed with incomplete number of acquisions.
			This value is in microseconds.
		*/
		int triggerRelaxationTime;
		/**
			Number of encoder clocks between trigger position and light barrier.
		*/
		int triggerOffset;
	};

	struct SetParams: public UnitParams
	{
		enum
		{
			Id = 370
		};

		typedef void Result;
	};

	struct SetMode
	{
		enum
		{
			Id = SetParams::Id + 1
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

		struct Result
		{
			IDriver::NativeTimer nativeTimestamp;
		};
	};

	struct PopId
	{
		enum
		{
			Id = SingleTrigger::Id + 1
		};

		struct Result
		{
			I_DWORD inspectionId;
			IDriver::NativeTimer nativeTimestamp;
			I_DWORD objectIndex;		// only inspected objects are used
			I_DWORD objectPosition;	// line base position (in counter ticks) of this object
		};
	};

	struct SetResult
	{
		enum
		{
			Id = PopId::Id + 1
		};

		struct Result{
			bool wasSet;
		};

		I_DWORD inspectionId;
		I_DWORD result;
		I_SDWORD ejectorIndex;
	};
};


} // namespace ilolv


#endif // !ilolv_CInspectionUnitMessages_included


