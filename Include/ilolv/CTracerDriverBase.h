#ifndef ilolv_CTracerDriverBase_included
#define ilolv_CTracerDriverBase_included


#include "ilolv/IDriver.h"
#include "ilolv/CTracerMessages.h"


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
		MAX_STATIONS = 4,
		MAX_EJECTORS = 4
	};

	enum InspectionResult
	{
		IR_OK,
		IR_WARNING,
		IR_ERROR
	};

	enum ControllerMode
	{
		CM_MANUAL,
		CM_AUTOMATIC,
		CM_MEASUREMENT
	};

	enum IoBit
	{
		IB_OK_SIGNAL,
		IB_NOK_SIGNAL,
		IB_LAST = IB_NOK_SIGNAL
	};

	CTracerDriverBase();

	/**	ResetQueue all stations.
	 */
	virtual void ResetQueue();

	/**	Get actual line index.
	 *		@return	line index or negative value if this line index is unknown.
	 */
	int GetLineIndex() const;
	/**	Set actual line index.
	 *		@param	index	line index or negative value if this line index is unknown.
	 */
	void SetLineIndex(int index);

	/**	Gets number of stations.
	 */
	int GetStationsCount() const;
	/**
		Return true if ejection control is enabled.
	*/
	bool IsEjectionControlEnabled() const;
	/**	Get offset point ejection control to light barrier.
	 *		It is important if both ejection control are enabled, else it should be 0.
	 *		This offset use encoder ticks as unit.
	 */
	I_DWORD GetEjectionControlOffset() const;
	/**
		Get single inspection unit parameters.
	*/
	virtual const CInspectionUnitMessages::UnitParams& GetUnitParams(int unitIndex) const;
	/**
		Get number of ejectors for this line.
	*/
	int GetEjectorsCount() const;
	/**
		Get index of ejector used if no ejector was specified by application.
		This ejector will be used for not processed object.
		If it negative, no ejection will be done.
	*/
	int GetAutonomeEjectorIndex() const;
	/**	Get minimal objects distance in encoder ticks.
	 */
	I_DWORD GetMinObjectsDistance() const;
	/**	Get tolerance of light barrier position.
	 *		This tolerance is used to assign ligt barrier event to object.
	 */
	I_DWORD GetPositionTolerance() const;
	/**	Get duration time of I/O bit for customer PLC.
	 */
	int GetIoBitDuration() const;
	/**
		Get parameters of single ejector.
	*/
	const CTracerMessages::EjectorParams& GetEjectorParams(int unitIndex) const;

	/**	Pop new id of object.
	 */
	int PopInspectionId(int unitIndex);
	/**	Gets base position of specified fifo object.
	 */
	I_DWORD GetObjectPosition(int inspectionId) const;
	/**	Gets object index counted since InitAllStations() was called.
	 */
	I_DWORD GetObjectIndex(int inspectionId) const;
	/**	Sets result of inspection.
	 */
	bool SetInspectionResult(int unitIndex, int inspectionId, int ejectorIndex);
	/**	Checks production line mode.
	 *		@sa	ControllerMode
	 */
	int GetControllerMode() const;

	/**	Called when input was changed.
	 *		@param	edgeBits	edge bits.
	 */
	void OnLightBarrierEdge(I_DWORD edgeBits);
	/**	Called if line position defined in SetNextEventPosition was reached.
	 *		@param	eventIndex	index of event used by InsertPositionToQueue().
	 */
	void OnPositionEvent(int eventIndex);

	// reimplemented (ilolv::IDriver)
	virtual bool OnInstruction(
				I_DWORD instructionCode,
				const void* instructionBuffer,
				int instructionBufferSize,
				void* responseBuffer,
				int responseBufferSize,
				I_DWORD& responseSize);
	virtual void OnPulse();

protected:
	enum TriggerType{
		TT_UNKNOWN,
		TT_LOST,
		TT_TRIGGER
	};

	enum ObjectState{
		OS_INIT,
		OS_DECIDED,
		OS_EJECTED,
		OS_CONTROLLED,
		OS_DIAGNOSTIC
	};

	struct InspectionUnitElement: public CInspectionUnitMessages::UnitParams
	{
		/**	Timestamp of last trigger.
		 */
		__int64 lastTriggerTime;
		/**	If true, trigger bit should be pulled down on time out.
		 */
		bool pullTriggerDown;
		/**	Index in FIFO of last triggered element.
		 */
		int lastTriggeredIndex;
		/** Index of next object can be inspected.
		 */
		int toInspectFifoIndex;
		/**	Last position of accepted edge.
		 */
		I_DWORD lastEdgePosition;
	};

	struct FifoElement{
		NativeTimer nativeTimeStamps[MAX_STATIONS];
		I_DWORD objectIndex;
		TriggerType usedTriggers[MAX_STATIONS];
		/**	Position of the base point before first station.
		 */
		I_DWORD basePosition;
		int okCount;				// number of OK objects, or -1 if this ejection decision was made
		int ejectorIndex;			// Id of ejector will be used if object should be ejected.
		ObjectState objectState;	// State of object.
	};

	struct EjectorInfo: public CTracerMessages::EjectorParams
	{
		/**	Counts ejector on sets.
		 */
		int ejectorOnCount;
		__int64 lastEjectorTime;
	};

	/**	Add object to fifo list.
	 */
	bool AddObjectToFifo(I_DWORD basePosition);
	/**	Find object index for specified position using tolerances.
	 *		@param	basePosition	position of the base.
	 *		@return					object FIFO index or -1.
	 */
	int FindInFifo(I_DWORD basePosition);
	/**	Remove last object from FIFO.
	 */
	const FifoElement& PopFifoLast();

	// position events
	void OnPositionTrigger(int unitIndex);
	void OnPositionEjectionDecision();
	void OnPointControl();
	void OnPositionEjectorOn(int ejectorIndex);
	void OnPositionEjectorOff(int ejectorIndex);
	void OnPositionPopFifo();

	// instruction processing
	/**
		Process single trigger instruction.
	*/
	void OnSingleTriggerInstruction(
				const CTracerMessages::SingleTrigger& instruction,
				CTracerMessages::SingleTrigger::Result& result);

	// abstract methods
	/**
		Set number of needed counter queues.
		Single queue generate interrupt if first in queue counter value is reached.
		After this first element will be removed from queue.
	*/
	virtual bool SetCounterQueuesCount(int count) = 0;
	/**	Set position of new position event.
	 *		This position will be pushed to event list.
	 *		Events for one index must be ordered by position.
	 *		When this position is reached, OnPostionEvent() should be called.
	 *		@params	index	index of event, it cannot be bigger than set using
	 *		SetCounterQueuesCount(int). For each index will be separated queue used.
	 */
	virtual void InsertPositionToQueue(int queueIndex, I_DWORD counterPosition) = 0;
	/**	Set trigger bit to specified state.
	 */
	virtual void SetTriggerBit(int triggerIndex, bool state) = 0;
	/**	Set state of ejector bit.
	 */
	virtual void SetEjectorBit(int ejectorIndex, bool state) = 0;
	/**	Set I/O bit to specified state.
	 *		@sa IoBit
	 */
	virtual void SetIoBit(int bitIndex, bool state) = 0;
	/**	Get actual base position of production line.
	 */
	virtual I_DWORD GetLinePosition() const = 0;
	/**	Get state of ejection control light barrier bit state.
	 */
	virtual bool GetEjectionControlBit() const = 0;
	/**	Sets message.
	 *		@param	category	message category, like in CMultiLineObjectTracerMessage::Category.
	 *							If it is -1, there is no message.
	 *		@param	id			Id of this message.
	 *		@param	errorTxt	pointer to C message string.
	 */
	virtual void SendMessage(int category, int id, const char* text, int* valuesPtr = NULL, int paramsCount = 0) = 0;

	// static methods
	static int CalcNextFifoIndex(int prevIndex);
	static int CalcPrevFifoIndex(int nextIndex);

private:
	enum{
		FIFO_INDEX_MASK = 255,
		FIFO_COUNT = FIFO_INDEX_MASK + 1
	};

	/** Position event Id's.
	 *		The bigger number means bigger priority.
	 */
	enum PositionEvent{
		/**	Indicate position where elements will be removed from FIFO.
		 */
		PE_POP_FIFO,
		/**	Indicate position where ejection decision must be taken.
		 */
		PE_EJECTION_DECISION,
		PE_POINT_CONTROL,
		PE_FIRST_TRIGGER
	};

		CTracerMessages::TracerParams m_params;

	int m_lineIndex;

	int m_controllerMode;

	int m_fifoLastIndex;	// inclusive
	int m_fifoNextIndex;	// exclusive
	int m_nextTriggerOffsetIndex;
	int m_ejectionDecisionIndex;
	int m_pointControlIndex;
	I_DWORD m_lastInspectedObjectIndex;
	I_DWORD m_lastFoundObjectIndex;

	int m_ejectionDecisionOffset;	// distance between base position and position where decision is made which (if any) ejector will be used.
	int m_popFifoOffset;			// distance between base position and end position, where objects are removed from fifo.

	InspectionUnitElement m_inspectionUnits[MAX_STATIONS];
	EjectorInfo m_ejectors[MAX_EJECTORS];
	FifoElement m_fifo[FIFO_COUNT];

	int m_firstEjectorOnEvent;
	int m_firstEjectorOffEvent;

	int m_firstStationIndex;

	__int64 m_lastIoBitTimes[IB_LAST + 1];
};


// inline methods

inline int CTracerDriverBase::GetControllerMode() const
{
	return m_controllerMode;
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


