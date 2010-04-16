#ifndef iorn_IObjectron_included
#define iorn_IObjectron_included


// ACF includes
#include "iproc/IProcessor.h"

#include "iorn/iorn.h"


namespace iorn
{


class CHypothesesSet;


/**
	Interface defining objectron.
	Objectron is something like layer in neural network, it has similiar behaviuor on interface level.
	It can calculate the output from input.
	It can be leaned self-associative or based on teached learning - it depends on implementation.
	The main difference to ANN is that objectron defines no assumption about internal realisation of this interface.
	It means classic algebraic realisation can be used, some recursive combinations of slave objectrons etc.
*/
class IObjectron: virtual public iproc::IProcessor
{
public:
	enum QualityQuery
	{
		/**
			Autoassociative learning.
		*/
		QQ_AA_LEARNING,
		/**
			Learning with teacher.
		*/
		QQ_TEACHER
	};

	/**
		Get output hypothesis set.
		This set is the product of objectron processing.
	*/
	virtual int ProcessHypotheses(
				const iprm::IParamsSet* paramsPtr,
				const CHypothesesSet& input,
				CHypothesesSet& output) const = 0;

	/**
		Get approximation of input hypothesis set from the output.
	*/
	virtual int BackPropagateHypotheses(
				const iprm::IParamsSet* paramsPtr,
				const CHypothesesSet& input,
				CHypothesesSet& output) const = 0;

	/**
		Do autoassociative training process.
		\param	paramsPtr			set of external parameters used to control train process.
		\param	inputLearningSet	set of learning input examples.
		\param	isProgressive		if true, the progressive learning is enabled.
	*/
	virtual int DoTrainAutoassoc(
				const iprm::IParamsSet* paramsPtr,
				const ILearningSet& inputLearningSet,
				bool isProgressive = false) = 0;

	/**
		Do training process with a teacher.
		\param	paramsPtr				set of external parameters used to control train process.
		\param	inputLearningSet		set of learning input examples.
		\param	outputLearningSet		set of learning output examples.
		\param	isProgressive			if true, the progressive learning is enabled.
	*/
	virtual int DoTrainWithTeacher(
				const iprm::IParamsSet* paramsPtr,
				const ILearningSet& inputLearningSet,
				const ILearningSet& outputLearningSet,
				bool isProgressive = false) = 0;

	/**
		Do training process with a additional error information.
		\param	paramsPtr				set of external parameters used to control train process.
		\param	inputLearningSet		set of learning input examples.
		\param	outputErrorSet			set of informations about output error.
										This information is represented as \c ILearningSet
										with only one 'sample' in data seuence.
										This value is interpreted as error value.
		\param	isProgressive			if true, the progressive learning is enabled.
	*/
	virtual int DoTrainWithError(
				const iprm::IParamsSet* paramsPtr,
				const ILearningSet& inputLearningSet,
				const ILearningSet& outputErrorSet,
				bool isProgressive = false) = 0;
};


} // namespace iorn


#endif // !iorn_IObjectron_included


