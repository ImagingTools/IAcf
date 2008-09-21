#ifndef ilolv_CInspectionUnitMessages_included
#define ilolv_CInspectionUnitMessages_included


#include "ilolv/ilolv.h"


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

	struct UnitParams
	{
		/**
			Number of microseconds should be trigger active.
		*/
		I_DWORD triggerDuration;
		/**
			Relaxation time needed to send next trigger.
			This can usefull for example to block new camera trigger before complete acquision of previos image is done.
			Please note, that in this case object will be processed with incomplete number of acquisions.
			This value is in microseconds.
		*/
		I_DWORD triggerRelaxationTime;
		/**
			Number of encoder clocks between light barrier and imaginated base position.
		*/
		I_DWORD stationOffset;
		/**
			Number of encoder clocks between trigger position and light barrier.
		*/
		I_DWORD triggerOffset;
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


