#ifndef iorn_CSigmoidalObjectronComp_included
#define iorn_CSigmoidalObjectronComp_included


// ACF includes
#include "imath/CVarMatrix.h"

#include "iorn/IObjectron.h"


namespace iorn
{


class CSigmoidalObjectronComp: virtual public IObjectron
{
public:
	// reimplemented (iorn::IObjectron)
	virtual double GetOperationQuality(int qualityQuery) const;
	virtual bool ProcessHypotheses(
				const CHypothesesSet& inputSet,
				CHypothesesSet& outputSet) const;
	virtual bool BackpropagateHypotheses(
				const CHypothesesSet* inputSetPtr,
				const CHypothesesSet& outputSet,
				CHypothesesSet& approxInputSet) const;
	virtual bool BackpropagateError(
				const CHypothesesSet& inputSet,
				double outputError,
				double& approxInputError,
				double& approxQuality) const;
	virtual int DoLearning(
				const ILearningSet& learningSet,
				const ILearnFeedback* learnFeedbackPtr,
				bool isProgressive = false);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	imath::CVarMatrix m_transformMatrix;
};


} // namespace iorn


#endif // !iorn_CSigmoidalObjectronComp_included


