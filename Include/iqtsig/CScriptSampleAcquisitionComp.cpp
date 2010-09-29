#include "iqtsig/CScriptSampleAcquisitionComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "iproc/IProgressManager.h"

// ACF-Solution includes
#include "imeas/IDataSequence.h"


namespace iqtsig
{


CScriptSampleAcquisitionComp::CScriptSampleAcquisitionComp()
:	m_frameNumber(0)
{
}


// reimplemented (iproc::IProcessor)

int CScriptSampleAcquisitionComp::DoProcessing(
			const iprm::IParamsSet* /*paramsPtr*/,
			const istd::IPolymorphic* /*inputPtr*/,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* progressManagerPtr)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	QScriptValue frameValue(&m_scriptEngine, ++m_frameNumber);
	m_scriptEngine.globalObject().setProperty("frame", frameValue);

	QString functionScript = iqt::GetQString(*m_defaultScriptAttrPtr);
	m_scriptEngine.evaluate(functionScript);

	istd::TChangeNotifier<imeas::IDataSequence> containerPtr(dynamic_cast<imeas::IDataSequence*>(outputPtr));
	if (!containerPtr.IsValid()){
		return TS_INVALID;
	}

	int samplesCount;
	if (m_samplesCountAttrPtr.IsValid()){
		samplesCount = *m_samplesCountAttrPtr;
		if (!containerPtr->CreateSequence(samplesCount, 1)){
			return TS_INVALID;
		}
	}
	else{
		samplesCount = containerPtr->GetSamplesCount();
	}

	QScriptValue calcCtor = m_scriptEngine.evaluate("Calc");

	int progressSessionId = -1;
	if (progressManagerPtr != NULL){
		progressSessionId = progressManagerPtr->BeginProgressSession("SampleCalc", "Calculate sampling", true);
	}

	int retVal = TS_OK;

	for (int i = 0; i < samplesCount; ++i){
		QScriptValueList arguments;
		QScriptValue xValue(&m_scriptEngine, i);
		arguments << xValue;

		double sample = calcCtor.call(m_scriptEngine.nullValue(), arguments).toNumber();

		if (m_scriptEngine.hasUncaughtException()) {
			retVal = TS_INVALID;

			break;
		}

		containerPtr->SetSample(i, 0, sample);

		if ((progressSessionId >= 0) && ((i % 100) == 50)){
			I_ASSERT(progressManagerPtr != NULL);

			if (progressManagerPtr->IsCanceled(progressSessionId)){
				retVal = TS_CANCELED;

				break;
			}

			progressManagerPtr->OnProgress(progressSessionId, double(i + 1) / samplesCount);
		}
	}

	if (progressSessionId >= 0){
		I_ASSERT(progressManagerPtr != NULL);

		progressManagerPtr->EndProgressSession(progressSessionId);
	}

	return TS_OK;
}


// reimplemented (isig::ISamplingConstraints)

istd::CRange CScriptSampleAcquisitionComp::GetIntervalRange() const
{
	return istd::CRange(0.001, 1);
}


bool CScriptSampleAcquisitionComp::IsSamplingModeSupported(int /*mode*/) const
{
	return true;
}


istd::CRange CScriptSampleAcquisitionComp::GetValueRange(bool /*forInput*/, bool /*forOutput*/, const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	return istd::CRange(*m_minValueAttrPtr, *m_maxValueAttrPtr);
}


int CScriptSampleAcquisitionComp::GetMaximalSamplesCount(bool /*forInput*/, bool /*forOutput*/, const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	return -1;
}


// reimplemented (icomp::IComponent)

void CScriptSampleAcquisitionComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	QScriptValue minValue(&m_scriptEngine, *m_minValueAttrPtr);
	m_scriptEngine.globalObject().setProperty("minValue", minValue);
	QScriptValue maxValue(&m_scriptEngine, *m_maxValueAttrPtr);
	m_scriptEngine.globalObject().setProperty("maxValue", maxValue);
}


} // namespace iqtsig


