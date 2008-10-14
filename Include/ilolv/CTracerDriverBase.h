#ifndef ilolv_CTracerDriverBase_included
#define ilolv_CTracerDriverBase_included


#include "ilolv/IDriver.h"
#include "ilolv/TOrderedPositionsQueue.h"
#include "ilolv/CTracerCommands.h"


namespace ilolv
{


/**
	Object tracer for hardware driver.
*/
class CTracerDriverBase: virtual public IDriver
{
public:
	enum
	{
		MAX_UNITS_COUNT = 4,
		MAX_EJECTORS = 4,
		MAX_LIGHT_BARRIERS = MAX_UNITS_COUNT * 2
	};

	CTracerDriverBase();

	/**
		Get global params for this tracer.
	*/
	const CTracerCommands::TracerParams& GetTracerParams() const;

	/**
		Reset traced object queue.
	*/
	virtual void ResetQueue();

	/**
		Get single inspection unit parameters.
	*/
	virtual const CInspectionUnitCommands::UnitParams& GetUnitParams(int unitIndex) const;

	/**
		Get parameters of single ejector.
	*/
	const CTracerCommands::EjectorParams& GetEjectorParams(int unitIndex) const;

	/**
		Processing of pop inspection command.
	*/
	int OnPopInspectionCommand(int unitIndex);
	/**
		Processing of set result command.
	*/
	bool OnSetResultCommand(int unitIndex, int inspectionId, int ejectorIndex);

	/**
		Process of position event.
		\param	eventIndex	index of event.
		\param	userContext	user context used by event adding.
	*/
	void ProcessPositionEvent(int eventIndex, void* userContext);

	// reimplemented (ilolv::IDriver)
	virtual bool OnCommand(
				int commandCode,
				const void* commandBuffer,
				int commandBufferSize,
				void* responseBuffer,
				int responseBufferSize,
				int& responseSize);
	virtual void OnHardwareInterrupt(I_DWORD interruptFlags);

protected:
	enum InspectionState
	{
		/**
			Initialized.
		*/
		IS_INIT,
		/**
			Trigger yould not be done.
		*/
		IS_UNTRIGGERED,
		/**
			Triggered, waiting to be get to inspect.
		*/
		IS_TRIGGERED,
		/**
			During inspection.
		*/
		IS_INSPECTION,
		/**
			Inspection is done and result is set.
		*/
		IS_RESULT
	};

	/**
		Describe state of traced object during processing in objects queue.
	*/
	enum ObjectState
	{
		/**
			Object initialized (the state after adding to queue).
		*/
		OS_INIT,
		/**
			Ejection decision was made.
			The value of ejectorIndex was calculated and it is valid.
		*/
		OS_EJECTION_DECIDED,
		/**
			Controlling of ejection is made.
		*/
		OS_CONTROLLED
	};

	struct InspectionUnitElement: public CInspectionUnitCommands::UnitParams
	{
		/**
			Time of last trigger on.
		*/
		__int64 triggerOnTime;
		/**
			Indicate that trigger bit is on.
		*/
		bool isTriggerBitSet;
		/**
			Position of rising light barier edge.
		*/
		I_DWORD edgeOnPosition;
		/**
			Last read barrier state.
		*/
		bool lastBarrierState;
	};

	/**
		Describe single inspection for each unit in objects queue.
	*/
	struct InspectionInfo
	{
		/**
			State inspection.
		*/
		InspectionState inspectionState;
		/**
			Ejector index set by application as inspection result.
		*/
		int ejectorIndex;
		/**
			Trigger time stamp.
		*/
		NativeTimer nativeTimeStamp;
	};

	struct ObjectInfo
	{
		/**
			State of this object.
		*/
		ObjectState objectState;

		/**
			State of inspection for each inspection unit.
		*/
		InspectionInfo units[MAX_UNITS_COUNT];

		/**
			Global object ID.
		*/
		I_DWORD objectIndex;

		/**
			Index of ejector used to eject this object.
		*/
		int decidedEjectorIndex;
	};

	struct EjectorInfo: public CTracerCommands::EjectorParams
	{
		/**
			Time when ejector was on.
		*/
		__int64 ejectionOnTime;
		/**
			Counter used to ensure that overlapped ejection will be done once.
		*/
		int overloadCounter;
	};

	/**
		Add object to fifo list.
	*/
	int PushNewObject(I_DWORD basePosition);

	/**
		Called when light barrier edge was detected.
		\param	basePosition	base line position.
								This position can differ from current position.
		\param	unit			inspection unit associated with this light barrier.
	*/
	void OnLightBarrierEdge(I_DWORD basePosition, int unitIndex);

	// position events
	void OnDecisionEvent(int inspectionIndex);
	void OnEjectionControlEvent(int inspectionIndex);
	void OnPopFifoEvent();
	void OnTriggerEvent(int unitIndex, int inspectionIndex);
	void OnEjectorOnEvent(int ejectorIndex);
	void OnEjectorOffEvent(int ejectorIndex);

	// command processing
	/**
		Process single trigger command.
	*/
	void OnSingleTriggerCommand(
				const CTracerCommands::SingleTrigger& command,
				CTracerCommands::SingleTrigger::Result& result);

	// abstract methods
	/**
		Get current position of production line read from encoder.
	*/
	virtual I_DWORD GetLinePosition() const = 0;
	/**
		Set number of needed counter queues.
		Single queue generate interrupt if first in queue counter value is reached.
		After this first element will be removed from queue.
	*/
	virtual bool SetCounterQueuesCount(int count) = 0;
	/**
		Insert new position to specified counter queue.
	*/
	virtual void InsertPositionToQueue(int queueIndex, I_DWORD counterPosition, void* userContext) = 0;
	/**
		Get state of specified light barrier bit.
	*/
	virtual bool GetLightBarrierBit(int lightBarrierIndex) const = 0;
	/**
		Turn specified trigger bit on or off.
	*/
	virtual void SetTriggerBit(int triggerIndex, bool state) = 0;
	/**
		Turn specified ejector bit on or off.
	*/
	virtual void SetEjectorBit(int ejectorIndex, bool state) = 0;

	// static methods
	static int CalcNextFifoIndex(int prevIndex);
	static int CalcPrevFifoIndex(int nextIndex);

private:
	enum{
		FIFO_INDEX_MASK = 255,
		FIFO_COUNT = FIFO_INDEX_MASK + 1
	};

	/**
		Indices of position events.
	*/
	enum EventIndex
	{
		/**
			Indicate position where ejection decision must be taken.
		*/
		EI_DECISION,
		EI_EJECTION_CONTROL,
		/**
			Indicate position where elements will be removed from FIFO.
		*/
		EI_REMOVE_FROM_QUEUE,
		/**
			Begin of trigger indices.
		*/
		EI_TRIGGER
	};

	CTracerCommands::TracerParams m_params;

	int m_controllerMode;

	I_DWORD m_lastInspectedObjectIndex;

	typedef TOrderedPositionsQueue<ObjectInfo> ObjectsFifo;
	ObjectsFifo m_objectsFifo;

	InspectionUnitElement m_inspectionUnits[MAX_UNITS_COUNT];
	EjectorInfo m_ejectors[MAX_EJECTORS];
	int m_lightBarriersOffset[MAX_LIGHT_BARRIERS];

	int m_decisionEventPosition;
	int m_queueEndPosition;

	int m_ejectorOnEventIndex;
	int m_ejectorOffEventIndex;

	int m_firstStationIndex;
};


// inline methods

inline const CTracerCommands::TracerParams& CTracerDriverBase::GetTracerParams() const
{
	return m_params;
}


// protected inline methods

// static methods

inline int CTracerDriverBase::CalcNextFifoIndex(int prevIndex)
{
	return FIFO_INDEX_MASK & (prevIndex + 1);
}


inline int CTracerDriverBase::CalcPrevFifoIndex(int nextIndex)
{
	return FIFO_INDEX_MASK & (nextIndex + (FIFO_COUNT - 1));
}


} // namespace ilolv


#endif // !ilolv_CTracerDriverBase_included


