#include "CMeilhausSimpleComp.h"


#include <QtCore/QTime>

#include "CChannelSelectionNode.h"


namespace imebase
{


CMeilhausSimpleComp::CMeilhausSimpleComp()
{
	meOpen(0);

	m_lastTaskId = -1;
}


// reimplemented (iproc::IProcessor)

int CMeilhausSimpleComp::GetProcessorState(const iprm::IParamsSet* /*paramsPtr*/) const
{
	return PS_READY;
}


bool CMeilhausSimpleComp::AreParamsAccepted(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			const istd::IChangeable* outputPtr) const
{
	if ((inputPtr != NULL) && (dynamic_cast<const imeas::IDataSequence*>(inputPtr) == NULL)){
		return false;
	}

	if ((outputPtr != NULL) && (dynamic_cast<const imeas::IDataSequence*>(outputPtr) == NULL)){
		return false;
	}

	CMeAddr address;
	return		GetChannelAddress(paramsPtr, address) &&
				(GetSamplingParams(paramsPtr) != 0);
}


int CMeilhausSimpleComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* /*progressManagerPtr*/)
{
	int taskId = BeginTask(paramsPtr, inputPtr, outputPtr);

	if (taskId < 0){
		return TS_INVALID;
	}

	return WaitTaskFinished(taskId);
}


int CMeilhausSimpleComp::BeginTask(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* /*progressManagerPtr*/)
{
	const imeas::IDataSequence* inputContainerPtr = dynamic_cast<const imeas::IDataSequence*>(inputPtr);
	imeas::IDataSequence* outputContainerPtr = dynamic_cast<imeas::IDataSequence*>(outputPtr);

	CMeAddr address;
	if (!GetChannelAddress(paramsPtr, address)){
		return -1;
	}

	const isig::ISamplingParams* samplingParamsPtr = GetSamplingParams(paramsPtr);
	if (samplingParamsPtr == NULL){
		return -1;
	}

	CMeContext* contextPtr = NULL;
	if (*m_isOutputAttrPtr){
		if (inputContainerPtr == NULL){
			return -1;
		}

		contextPtr = new CMeContext(address, *m_isOutputAttrPtr, const_cast<imeas::IDataSequence*>(inputContainerPtr));

		contextPtr->CopyFromContainer();
	}
	else{
		if (outputContainerPtr == NULL){
			return -1;
		}

		outputContainerPtr->CreateSequence(1024, 1);

		contextPtr = new CMeContext(address, *m_isOutputAttrPtr, outputContainerPtr);
	}

	if (!contextPtr->Register(samplingParamsPtr->GetInterval())){
		delete contextPtr;

		return -1;
	}

	PullNextTaskId();
	m_activeTaskList[m_lastTaskId] = contextPtr;

	return m_lastTaskId;

}


int CMeilhausSimpleComp::WaitTaskFinished(int taskId, double timeoutTime, bool killOnTimeout)
{
	int retVal = TS_OK;

	if (taskId >= 0){
		TasksList::iterator foundIter = m_activeTaskList.find(taskId);
		if (foundIter != m_activeTaskList.end()){
			CMeContext* contextPtr = foundIter->second;
			I_ASSERT(contextPtr != NULL);

			retVal = WaitSingleTaskFinished(*contextPtr, timeoutTime, killOnTimeout);
			if (retVal != TS_WAIT){
				delete contextPtr;

				m_activeTaskList.erase(foundIter);
			}
		}
	}
	else{
		double localTimeout = timeoutTime;
		QTime stopper;
		stopper.start();

		for (		TasksList::iterator iter = m_activeTaskList.begin();
					iter != m_activeTaskList.end();){
			CMeContext* contextPtr = iter->second;
			I_ASSERT(contextPtr != NULL);

			int taskState = WaitSingleTaskFinished(*contextPtr, localTimeout, killOnTimeout);
			if (taskState != TS_OK){
				retVal = taskState;
			}

			TasksList::iterator deleteIter = iter++;

			if (taskState != TS_WAIT){
				delete contextPtr;

				m_activeTaskList.erase(deleteIter);
			}

			localTimeout -= (double)stopper.elapsed() / 1000;

			if (localTimeout < 0){
				localTimeout = 0;
			}
		}
	}

	return retVal;
}


void CMeilhausSimpleComp::CancelTask(int taskId)
{
	if (taskId >= 0){
		TasksList::iterator foundIter = m_activeTaskList.find(taskId);
		if (foundIter != m_activeTaskList.end()){
			CMeContext* contextPtr = foundIter->second;
			I_ASSERT(contextPtr != NULL);

			delete contextPtr;

			m_activeTaskList.erase(foundIter);
		}
	}
	else{
		for (		TasksList::const_iterator iter = m_activeTaskList.begin();
					iter != m_activeTaskList.end();
					++iter){
			CMeContext* contextPtr = iter->second;
			I_ASSERT(contextPtr != NULL);

			delete contextPtr;
		}

		m_activeTaskList.clear();
	}
}


int CMeilhausSimpleComp::GetReadyTask()
{
	for (		TasksList::const_iterator iter = m_activeTaskList.begin();
				iter != m_activeTaskList.end();
				++iter){
		CMeContext* contextPtr = iter->second;
		I_ASSERT(contextPtr != NULL);

		if (contextPtr->IsDone()){
			return iter->first;
		}
	}

	return -1;
}


int CMeilhausSimpleComp::GetTaskState(int taskId) const
{
	CMeContext* contextPtr = NULL;

	if (taskId >= 0){
		TasksList::const_iterator foundIter = m_activeTaskList.find(taskId);
		if (foundIter != m_activeTaskList.end()){
			contextPtr = foundIter->second;
		}
	}
	else{
		TasksList::const_reverse_iterator lastIter = m_activeTaskList.rbegin();
		if (lastIter != m_activeTaskList.rend()){
			contextPtr = lastIter->second;
		}
	}

	if ((contextPtr != NULL) && !contextPtr->IsDone()){
		return TS_WAIT;
	}

	return TS_OK;
}


void CMeilhausSimpleComp::InitProcessor(const iprm::IParamsSet* /*paramsPtr*/)
{
}


// reimplemented (imebase::IMeilhausAccess)

bool CMeilhausSimpleComp::CreateSelectionTree(CChannelSelectionNode& result) const
{
	int devicesCount;
	if (meQueryNumberDevices(&devicesCount) != 0){
		return false;
	}

	for (int deviceIndex = 0; deviceIndex < devicesCount; deviceIndex++){
		istd::TDelPtr<CChannelSelectionNode> deviceNodePtr(new CChannelSelectionNode);

		int  meType = (*m_isOutputAttrPtr)? ME_TYPE_AO: ME_TYPE_AI;

		for (		int subdeviceIndex = 0;
					meQuerySubdeviceByType(deviceIndex, subdeviceIndex, meType, ME_SUBTYPE_STREAMING, &subdeviceIndex) == 0;
					++subdeviceIndex){
			istd::TDelPtr<CChannelSelectionNode> subdeviceNodePtr(new CChannelSelectionNode);

			int channelsCount;
			meQueryNumberChannels(deviceIndex, subdeviceIndex, &channelsCount);

			for (int channelIndex = 0; channelIndex < channelsCount; ++channelIndex){
				QString channelName = QString::number(channelIndex);
				subdeviceNodePtr->InsertNode(channelName, channelIndex, NULL);
			}

			if (subdeviceNodePtr->GetOptionsCount() > 0){
				QString subdeviceName = QString::number(subdeviceIndex);
				deviceNodePtr->InsertNode(subdeviceName, subdeviceIndex, subdeviceNodePtr.PopPtr());
			}
		}

		if (deviceNodePtr->GetOptionsCount() > 0){
			QString deviceName = QString::number(deviceIndex);
			char buffer[256];
			if (meQueryNameDevice(deviceIndex, buffer, 255) == 0){
				deviceName = deviceName + ":" + buffer;
			}

			result.InsertNode(deviceName, deviceIndex, deviceNodePtr.PopPtr());
		}
	}

	return true;
}


// reimplemented (isig::ISamplingConstraints)

istd::CRange CMeilhausSimpleComp::GetIntervalRange() const
{// Maximum speed is 500kHz, minimum 1Hz.
	return istd::CRange(0.000002, 1.000000);
}


bool CMeilhausSimpleComp::IsSamplingModeSupported(int mode) const
{
	if (*m_isOutputAttrPtr){
		return mode == isig::ISamplingParams::SM_PERIODIC;
	}
	else{
		return mode == isig::ISamplingParams::SM_SINGLE;
	}
}


istd::CRange CMeilhausSimpleComp::GetValueRange(bool /*forInput*/, bool /*forOutput*/, const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	return istd::CRange(-10, 9.996);
}


int CMeilhausSimpleComp::GetMaximalSamplesCount(bool /*forInput*/, bool /*forOutput*/, const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	return 1024;
}


// protected methods

int CMeilhausSimpleComp::PullNextTaskId()
{
	m_lastTaskId = (m_lastTaskId + 1) & 0x7fffffff;

	return m_lastTaskId;
}


bool CMeilhausSimpleComp::GetChannelAddress(const iprm::IParamsSet* paramsPtr, CMeAddr& result) const
{
	std::string selectionId = (*m_addressParamIdAttrPtr).toStdString();
	const iprm::ISelectionParam* selectionPtr = dynamic_cast<const iprm::ISelectionParam*>(paramsPtr->GetParameter(selectionId));

	const CChannelSelectionNode* deviceNodePtr = dynamic_cast<const CChannelSelectionNode*>(selectionPtr);
	if (deviceNodePtr != NULL){
		result.device = deviceNodePtr->GetActivePhysicalAddress();
		const CChannelSelectionNode* subdeviceNodePtr = dynamic_cast<const CChannelSelectionNode*>(deviceNodePtr->GetActiveSubselection());
		if (subdeviceNodePtr != NULL){
			result.subdevice = subdeviceNodePtr->GetActivePhysicalAddress();
			const CChannelSelectionNode* channelNodePtr = dynamic_cast<const CChannelSelectionNode*>(subdeviceNodePtr->GetActiveSubselection());
			if (channelNodePtr != NULL){
				result.channel = channelNodePtr->GetActivePhysicalAddress();

				return true;
			}
		}
	}

	return false;
}


const isig::ISamplingParams* CMeilhausSimpleComp::GetSamplingParams(const iprm::IParamsSet* paramsPtr) const
{
	std::string samplingId = (*m_samplingParamsIdAttrPtr).toStdString();

	return dynamic_cast<const isig::ISamplingParams*>(paramsPtr->GetParameter(samplingId));
}


int CMeilhausSimpleComp::WaitSingleTaskFinished(CMeContext& context, double timeoutTime, bool killOnTimeout)
{
	if (timeoutTime < 0){
		timeoutTime = context.GetInterval() + 1;
	}

	if (context.Wait(timeoutTime)){
		if (!*m_isOutputAttrPtr){
			context.CopyToContainer();
		}

		return TS_OK;
	}
	else{
		if (killOnTimeout){
			return TS_INVALID;
		}
		else{
			return TS_WAIT;
		}
	}
}


} // namespace imebase


