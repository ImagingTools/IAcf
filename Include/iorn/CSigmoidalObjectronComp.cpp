#include "iorn/CSigmoidalObjectronComp.h"


#include "iorn/CHypothesesSet.h"
#include "iorn/CHypothesisInfo.h"


namespace iorn
{


// reimplemented (iorn::IObjectron)

double CSigmoidalObjectronComp::GetOperationQuality(int qualityQuery) const
{
	switch (qualityQuery){
	case QQ_PROCESSING:
	case QQ_BACKPROPAGATION:
	case QQ_EXAMPLE_LEARNING:
		return 1;
	}

	return 0;
}


bool CSigmoidalObjectronComp::ProcessHypotheses(
			const CHypothesesSet& inputSet,
			CHypothesesSet& outputSet) const
{
	const CHypothesisInfo* inputInfoPtr = inputSet.GetInfo().GetPtr();
	if (inputInfoPtr == NULL){
		return false;
	}

	int inputSamplesCount = inputInfoPtr->GetSamplesCount();
	if (inputSamplesCount <= 0){
		return false;
	}

	istd::CIndex2d sizes = m_transformMatrix.GetSizes();

	int inputChannelsCount = inputInfoPtr->GetActiveChannelsCount();
	int inputElementsCount = inputChannelsCount * inputSamplesCount;
	if (inputElementsCount != sizes[0]){
		return false;
	}

	const CHypothesisInfo* outputInfoPtr = inputSet.GetInfo().GetPtr();
	if (outputInfoPtr == NULL){
		return false;
	}

	int outputSamplesCount = outputInfoPtr->GetSamplesCount();
	if (outputSamplesCount <= 0){
		return false;
	}

	int outputChannelsCount = outputInfoPtr->GetActiveChannelsCount();
	int outputElementsCount = outputChannelsCount * inputSamplesCount;
	if (inputElementsCount != sizes[1]){
		return false;
	}

	int hypothesisCount = inputSet.GetHypothesesCount();

	// TODO: implement it

	return false;
}


bool CSigmoidalObjectronComp::BackpropagateHypotheses(
			const CHypothesesSet* inputSetPtr,
			const CHypothesesSet& outputSet,
			CHypothesesSet& approxInputSet) const
{
	return false;
}


bool CSigmoidalObjectronComp::BackpropagateError(
			const CHypothesesSet& inputSet,
			double outputError,
			double& approxInputError,
			double& approxQuality) const
{
	return false;
}


int CSigmoidalObjectronComp::DoLearning(
			const ILearningSet& learningSet,
			const ILearnFeedback* learnFeedbackPtr,
			bool isProgressive)
{
	return false;
}


// reimplemented (iser::ISerializable)

bool CSigmoidalObjectronComp::Serialize(iser::IArchive& archive)
{
	return false;
}


} // namespace iorn


