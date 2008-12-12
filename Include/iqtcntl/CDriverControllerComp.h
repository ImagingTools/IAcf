#ifndef iqtcntl_CDriverControllerComp_included
#define iqtcntl_CDriverControllerComp_included


// ACF includes
#include "imod/CMultiModelObserverBase.h"
#include "icomp/CComponentBase.h"
#include "inat/CTimer.h"

#include "ilolv/ICommandCaller.h"
#include "ilolv/CMultiTracerCommands.h"

#include "icntl/IAutomaticController.h"
#include "icntl/IMultiLineController.h"
#include "icntl/ILineController.h"
#include "icntl/IInspectionUnitController.h"
#include "icntl/IEjectorController.h"
#include "icntl/IObjectInspection.h"
#include "icntl/IMultiLineParams.h"
#include "icntl/ILineParams.h"
#include "icntl/IInspectionUnitParams.h"
#include "icntl/IEjectorParams.h"

#include "iqtcntl/iqtcntl.h"


namespace iqtcntl
{


/**
	Synchronize driver with production line parameter and control working mode.
*/
class CDriverControllerComp:
			public icomp::CComponentBase,
			protected imod::CMultiModelObserverBase,
			virtual public icntl::IAutomaticController,
			virtual public icntl::IMultiLineController
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CDriverControllerComp);
		I_REGISTER_INTERFACE(icntl::IAutomaticController);
		I_REGISTER_INTERFACE(icntl::IMultiLineController);
		I_ASSIGN(m_multiLineParamsCompPtr, "MultiLineParams", "Description of parameters for all production lines", true, "MultiLineParams");
		I_ASSIGN(m_multiLineModelCompPtr, "MultiLineParams", "Description of parameters for all production lines", true, "MultiLineParams");
		I_ASSIGN(m_commandCallerCompPtr, "CommandCaller", "Send low level commands to driver", true, "CommandCaller");
		I_ASSIGN_MULTI_0(m_autonomeEjectorIndicesAttrPtr, "AutonomeEjectorIndices", "Indices of autonome ejectors for each line", false);
		I_ASSIGN_MULTI_0(m_ejectionControlIndicesAttrPtr, "EjectionControlIndices", "Indices of ejection control light barrier or -1 if disabled (for each line)", false);
		I_ASSIGN_MULTI_0(m_lightBarriersBitsAttrPtr, "LightBarrierFirstBits", "Indices of first light barrier input bit for each line", true);
		I_ASSIGN_MULTI_0(m_ejectorsBitsAttrPtr, "EjectorFirstBits", "Indices of first ejector output bit for each line", true);
		I_ASSIGN_MULTI_0(m_triggersBitsAttrPtr, "TriggerFirstBits", "Indices of first trigger output bit for each line", true);
		I_ASSIGN_MULTI_0(m_signalBitsAttrPtr, "SignaBits", "Indices of each signal bit, first 3 bits are reserved for heartbeat, error and warning bit", false);
		I_ASSIGN(m_triggerDurationAttrPtr, "TriggerDuration", "Duration of trigger pulse in seconds", true, 0.001);
		I_ASSIGN(m_triggerRelaxationAttrPtr, "TriggerRelaxationTime", "Minimal distance between two triggers (in seconds)", true, 0.04);
		I_ASSIGN(m_maxEjectorOnTimeAttrPtr, "MaxEjectionTime", "Maximal time ejection signal is on (in seconds)", true, 0.5);
		I_ASSIGN(m_heartbeatPeriodAttrPtr, "HeartbeatPeriod", "Heartbeat signal period in seconds", true, 0.1);
	I_END_COMPONENT;

	CDriverControllerComp();

	// reimplemented (icntl::IAutomaticController)
	virtual int GetWorkMode() const;
	virtual bool SetWorkMode(int mode);
	virtual bool IsModeSupported(int mode) const;

	// reimplemented (icntl::IMultiLineController)
	virtual int GetLinesCount() const;
	virtual icntl::ILineController& GetLineController(int lineIndex) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected:
	class ObjectInspection: virtual public icntl::IObjectInspection
	{
	public:
		ObjectInspection(
					ilolv::ICommandCaller* commandCallerPtr,
					int lineIndex,
					int stationIndex,
					const ilolv::CMultiTracerCommands::PopId::Result& objectParams);

		// reimplemented (icntl::IObjectInspection)
		virtual I_DWORD GetObjectId() const;
		virtual const isys::ITimer& GetTimestamp() const;
		virtual bool SetEjector(int ejectorIndex);

	private:
		ilolv::ICommandCaller& m_commandCaller;
		int m_lineIndex;
		int m_unitIndex;
		I_DWORD m_objectId;
		I_DWORD m_inspectionId;
		inat::CTimer m_timestamp;
	};

	class UnitController: virtual public icntl::IInspectionUnitController
	{
	public:
		// reimplemented (icntl::ILineController)
		virtual icntl::IObjectInspection* PopObjectInspection();
		virtual isys::ITimer* DoCameraTrigger();

		int unitIndex;
		int lineIndex;

		CDriverControllerComp* parentPtr;
	};
	typedef std::vector<UnitController> UnitControllers;

	class EjectorController: virtual public icntl::IEjectorController
	{
	public:
		// reimplemented (icntl::IEjectorController)
		virtual int GetEjectedCounter() const;
		virtual bool DoTestEjection();

		int ejectorIndex;
		int lineIndex;

		CDriverControllerComp* parentPtr;
	};
	typedef std::vector<EjectorController> EjectorControllers;

	class LineController: virtual public icntl::ILineController
	{
	public:
		// reimplemented (icntl::ILineController)
		virtual int GetInspectionUnitsCount() const;
		virtual icntl::IInspectionUnitController& GetInspectionUnitController(int unitIndex) const;
		virtual int GetEjectorsCount() const;
		virtual icntl::IEjectorController& GetEjectorController(int ejectorIndex) const;
		virtual bool GetTransmissionPosition(double& result) const;
		virtual bool GetObjectPosition(I_DWORD objectId, double& result) const;
		virtual bool GetLastObjectId(I_DWORD& result) const;

		int lineIndex;

		mutable UnitControllers unitControllers;
		mutable EjectorControllers ejectorControllers;

		double ticksPerDistanceUnit;

		CDriverControllerComp* parentPtr;
	};
	typedef std::vector<LineController> LineControllers;

	ilolv::ICommandCaller* GetCommandCaller() const;

	virtual bool SetParamsToDriver();
	virtual bool SetLineParamsToDriver(
				int lineIndex,
				const icntl::ILineParams& lineParams,
				LineController& controller);
	virtual bool SetUnitParamsToDriver(
				int lineIndex,
				int unitIndex,
				const icntl::IInspectionUnitParams& unitParams,
				double ticksPerDistUnit,
				UnitController& controller);
	virtual bool SetEjectorParamsToDriver(
				int lineIndex,
				int ejectorIndex,
				const icntl::IEjectorParams& ejectorParams,
				double ticksPerDistUnit,
				EjectorController& controller);

	// reimplemented (imod::CMultiModelObserverBase)
	virtual void OnUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	mutable LineControllers m_lineControllers;

	I_REF(icntl::IMultiLineParams, m_multiLineParamsCompPtr);
	I_REF(imod::IModel, m_multiLineModelCompPtr);
	I_REF(ilolv::ICommandCaller, m_commandCallerCompPtr);
	I_MULTIATTR(int, m_autonomeEjectorIndicesAttrPtr);
	I_MULTIATTR(int, m_ejectionControlIndicesAttrPtr);
	I_MULTIATTR(int, m_lightBarriersBitsAttrPtr);
	I_MULTIATTR(int, m_ejectorsBitsAttrPtr);
	I_MULTIATTR(int, m_triggersBitsAttrPtr);
	I_MULTIATTR(int, m_signalBitsAttrPtr);
	I_ATTR(double, m_triggerDurationAttrPtr);
	I_ATTR(double, m_triggerRelaxationAttrPtr);
	I_ATTR(double, m_maxEjectorOnTimeAttrPtr);
	I_ATTR(double, m_heartbeatPeriodAttrPtr);

	int m_workMode;
};


// inline methods

inline ilolv::ICommandCaller* CDriverControllerComp::GetCommandCaller() const
{
	return m_commandCallerCompPtr.GetPtr();
}


} // namespace iqtcntl


#endif // !iqtcntl_CDriverControllerComp_included


