#ifndef iorn_CHypothesesSet_included
#define iorn_CHypothesesSet_included


// STL includes
#include <vector>

// ACF includes
#include "istd/IChangeable.h"
#include "istd/TTransPtr.h"
#include "istd/TSmartPtr.h"

// ACF-Solutions incldues
#include "imeas/IDataSequence.h"

#include "iorn/CHypothesisInfo.h"


namespace iorn
{


/**
	Represent set of alternated state hypotheses.
	This is main data exchange object in world of objectrons.
*/
class CHypothesesSet: virtual istd::IChangeable
{
public:
	/**
		Construct empty hypothesis set.
	*/
	explicit CHypothesesSet(const istd::TTransPtr<CHypothesisInfo>& infoPtr);

	/**
		Construct hypothesis set containing one hypothesis.
	*/
	CHypothesesSet(const imeas::IDataSequence* hypothesisPtr, double weight, bool isOwned);

	/**
		Get data info.
		All depend hypothesis must use the same info.
	*/
	const istd::TTransPtr<CHypothesisInfo>& GetInfo() const;

	/**
		Get number of possible hypotheses.
	*/
	virtual int GetHypothesesCount() const;

	/**
		Set number of possible hypotheses.
		Please note, that new hypothesis will be initialized as NULL pointer.
	*/
	virtual void SetHypothesesCount(int count);

	/**
		Get access to single hypothesis.
		\param	index	index of hypothesis.
		\param	weight	output value of hypothesis weight.
		\return	hypothesis at specified index, or NULL if hypothesis was not set.
	*/
	virtual const imeas::IDataSequence* GetHypothesis(int index, double& weight) const;
	/**
		Set single hypothesis.
		\param	index			index of hypothesis.
		\param	hypothesisPtr	pointer to hypothesis object.
		\param	weight			hypothesis weight.
		\param	isOwned			indicate that the hypothesis is owned by this container and will be removed during its destruction.
	*/
	virtual bool SetHypothesis(int index, const imeas::IDataSequence* hypothesisPtr, double weight, bool isOwned = false);
	/**
		Get ownership of some hipothesis and reset it in container.
		\param	index	index of hypothesis.
		\param	weight	output value of hypothesis weight.
		\param	isOwned	output value of ownership flag.
		\return	hypothesis at specified index, or NULL if hypothesis was not set.
	*/
	virtual const imeas::IDataSequence* PopHypothesis(int index, double& weight, bool& isOwned);

	/**
		Get the best hypothesis in this set.
		\return	index of best hypothesis or negative value if this set has no active hypothesis.
	*/
	virtual int GetBestHypothesisIndex() const;

private:
	struct Element
	{
		double weight;
		const imeas::IDataSequence* hypothesisPtr;
		bool isOwned;
	};

	typedef std::vector<Element> Elements;

	Elements m_elements;

	istd::TSmartPtr<CHypothesisInfo> m_infoPtr;

	mutable bool m_isBestKnown;
	mutable int m_bestHypothesisIndex;

	static Element s_emptyElement;
};


} // namespace iorn


#endif // !iorn_CHypothesesSet_included


