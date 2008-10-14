#ifndef icntl_CDriverControllerComp_included
#define icntl_CDriverControllerComp_included


// ACF includes
#include "imod/CMultiModelObserverBase.h"
#include "icomp/CComponentBase.h"

#include "ilolv/ICommandCaller.h"

#include "icntl/IAutomaticController.h"
#include "icntl/IMultiLineParams.h"


namespace icntl
{


class ILineParams;
class IInspectionUnitParams;
class IEjectorParams;


/**
	Synchronize driver with production line parameter and control working mode.
*/
class CDriverControllerComp:
			public icomp::CComponentBase,
			protected imod::CMultiModelObserverBase,
			virtual public IAutomaticController
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CDriverControllerComp);
		I_REGISTER_INTERFACE(IAutomaticController);
		I_ASSIGN(m_multiLineParamsCompPtr, "MultiLineParams", "Description of parameters for all production lines", true, "MultiLineParams");
		I_ASSIGN(m_multiLineModelCompPtr, "MultiLineParams", "Description of parameters for all production lines", true, "MultiLineParams");
		I_ASSIGN(m_commandCallerCompPtr, "CommandCaller", "Send low level commands to driver", true, "CommandCaller");
		I_ASSIGN_MULTI_0(m_autonomeEjectorIndicesAttrPtr, "AutonomeEjectorIndices", "Indices of autonome ejectors for each line", false);
		I_ASSIGN_MULTI_0(m_lightBarriersBitsAttrPtr, "LightBarrierFirstBits", "Indices of first light barrier input bit for each line", true);
		I_ASSIGN_MULTI_0(m_ejectorsBitsAttrPtr, "EjectorFirstBits", "Indices of first ejector output bit for each line", true);
		I_ASSIGN_MULTI_0(m_triggersBitsAttrPtr, "TriggerFirstBits", "Indices of first trigger output bit for each line", true);
		I_ASSIGN(m_triggerDurationAttrPtr, "TriggerDuration", "Duration of trigger pulse in seconds", true, 0.001);
		I_ASSIGN(m_triggerRelaxationAttrPtr, "TriggerRelaxationTime", "Minimal distance between two triggers (in seconds)", true, 0.04);
		I_ASSIGN(m_maxEjectorOnTimeAttrPtr, "MaxEjectionTime", "Maximal time ejection signal is on (in seconds)", true, 0.5);
	I_END_COMPONENT;

	CDriverControllerComp();

	// reimplemented (icntl::IAutomaticController)
	virtual int GetWorkMode() const;
	virtual bool SetWorkMode(int mode);
	virtual bool IsModeSupported(int mode) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected:
	ilolv::ICommandCaller* GetCommandCaller() const;

	virtual bool SetParamsToDriver();
	virtual bool SetLineParamsToDriver(int lineIndex, const ILineParams& lineParams);
	virtual bool SetUnitParamsToDriver(
				int lineIndex,
				int unitIndex,
				const IInspectionUnitParams& unitParams,
				double ticksPerDistUnit);
	virtual bool SetEjectorParamsToDriver(
				int lineIndex,
				int ejectorIndex,
				const IEjectorParams& ejectorParams,
				double ticksPerDistUnit);

	// reimplemented (imod::CMultiModelObserverBase)
	virtual void OnUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	I_REF(IMultiLineParams, m_multiLineParamsCompPtr);
	I_REF(imod::IModel, m_multiLineModelCompPtr);
	I_REF(ilolv::ICommandCaller, m_commandCallerCompPtr);
	I_MULTIATTR(int, m_autonomeEjectorIndicesAttrPtr);
	I_MULTIATTR(int, m_lightBarriersBitsAttrPtr);
	I_MULTIATTR(int, m_ejectorsBitsAttrPtr);
	I_MULTIATTR(int, m_triggersBitsAttrPtr);
	I_ATTR(double, m_triggerDurationAttrPtr);
	I_ATTR(double, m_triggerRelaxationAttrPtr);
	I_ATTR(double, m_maxEjectorOnTimeAttrPtr);

	int m_workMode;
};


// inline methods

inline ilolv::ICommandCaller* CDriverControllerComp::GetCommandCaller() const
{
	return m_commandCallerCompPtr.GetPtr();
}


} // namespace icntl


#endif // !icntl_CDriverControllerComp_included


