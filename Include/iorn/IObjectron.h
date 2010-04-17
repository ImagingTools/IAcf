#ifndef iorn_IObjectron_included
#define iorn_IObjectron_included


// ACF includes
#include "iser/ISerializable.h"

#include "iorn/iorn.h"


namespace iorn
{


class CHypothesesSet;
class ILearningSet;
class ILearnFeedback;


/**
	Interface defining objectron.
	Objectron is something like layer in neural network, it has similiar behaviuor on interface level.
	It can calculate the output from input.
	It can be leaned self-associative or based on teached learning - it depends on implementation.
	The main difference to ANN is that objectron defines no assumption about internal realisation of this interface.
	It means classic algebraic realisation can be used, some recursive combinations of slave objectrons etc.
	Objectron is serializable, its state is determined by learning process.
*/
class IObjectron: virtual public iser::ISerializable
{
public:
	enum QualityQuery
	{
		/**
			General quality of this processing method.
		*/
		QQ_PROCESSING,
		/**
			Backpropagation of hypotheses without input data.
		*/
		QQ_BACKPROPAGATION,
		/**
			Backpropagation of hypotheses with input data.
		*/
		QQ_BACKPROPAGATION_INPUT,
		/**
			Backpropagation of error.
		*/
		QQ_ERROR_BACKPROPAGATION,
		/**
			Progressive learning.
		*/
		QQ_PROGRESSIVE_LEARNING,
		/**
			Unsupervised learning.
		*/
		QQ_UNSUPERVISED_LEARNING,
		/**
			Supervised learning with examples.
		*/
		QQ_EXAMPLE_LEARNING,
		/**
			Supervised learning with critic.
		*/
		QQ_CRITIC_LEARNING
	};

	virtual double GetOperationQuality(int qualityQuery) const = 0;

	/**
		Get output hypothesis set.
		This set is the product of objectron processing.
		\param	inputSet	set of possible input hypothesis.
		\param	outputSet	set of possible output hypothesis.
		\return	false if calculation of hypotheses failed.
	*/
	virtual bool ProcessHypotheses(
				const CHypothesesSet& inputSet,
				CHypothesesSet& outputSet) const = 0;

	/**
		Get approximation of input hypothesis set from the output and (optional) suggested input.
		\param	inputSetPtr		optional suggested input set.
		\param	outputSet		output set should used as input data for backpropagation.
		\param	approxInputSet	result of this operation - approximated input data set.
		\return	false if calculation of hypotheses failed or this operation is not supported.
	*/
	virtual bool BackpropagateHypotheses(
				const CHypothesesSet* inputSetPtr,
				const CHypothesesSet& outputSet,
				CHypothesesSet& approxInputSet) const = 0;

	/**
		Get approximation of input error propagated from output.
		\return	false if calculation of error failed or this operation is not supported.
	*/
	virtual bool BackpropagateError(
				const CHypothesesSet& inputSet,
				double outputError,
				double& approxInputError,
				double& approxQuality) const = 0;

	/**
		Do learning process.
		There are generally two kinds of learning: supervised and unsupervised.
		For supervised learning you have to provide \c ILearnFeedback object used to retrive supervisor information.
		\param	inputLearningSet	set of learning input examples.
		\param	learnFeedbackPtr	pointer to learn feedback. It is necessary for supervised learning.
		\param	isProgressive		if true, the progressive learning is enabled, it means only correction of current state should be done.
	*/
	virtual int DoLearning(
				const ILearningSet& learningSet,
				const ILearnFeedback* learnFeedbackPtr,
				bool isProgressive = false) = 0;
};


} // namespace iorn


#endif // !iorn_IObjectron_included


