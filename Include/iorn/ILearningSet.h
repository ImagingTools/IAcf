#ifndef iorn_ILearningSet_included
#define iorn_ILearningSet_included


// ACF includes
#include "istd/IChangeable.h"

#include "iorn/iorn.h"


namespace iorn
{


class CHypothesesSet;


/**
	Represent learning set.
	Learning set consists of set of hypothesis sets, one hypothesis set for one example.
*/
class ILearningSet: virtual istd::IChangeable
{
public:
	/**
		Get number of possible hypotheses.
	*/
	virtual int GetSetsCount() const = 0;

	/**
		Get access to single hypothesis set.
		\param	index	index of hypothesis set.
						For performance reasons the progressive access will be prefered.
		\param	weight	output value of weight of hypothesis set.
		\return	hypothesis set at specified index, or NULL if no hypothesis set can be accessed.
	*/
	virtual const CHypothesesSet* GetSingleSet(int index, double& weight) const;
};


} // namespace iorn


#endif // !iorn_ILearningSet_included


