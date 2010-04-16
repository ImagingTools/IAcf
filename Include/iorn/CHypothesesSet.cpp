#include "iorn/CHypothesesSet.h"


namespace iorn
{


CHypothesesSet::CHypothesesSet()
:	m_isBestKnown(true),
	m_bestHypothesisIndex(-1)
{
}


CHypothesesSet::CHypothesesSet(const imeas::IDataSequence* hypothesisPtr, double weight, bool isOwned)
:	m_isBestKnown(true)
{
	Element element = {weight, hypothesisPtr, isOwned};

	m_elements.push_back(element);
}


int CHypothesesSet::GetHypothesesCount() const
{
	return int(m_elements.size());
}


void CHypothesesSet::SetHypothesesCount(int count)
{
	m_elements.resize(count, s_emptyElement);
}


const imeas::IDataSequence* CHypothesesSet::GetHypothesis(int index, double& weight) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_elements.size()));

	const Element& element = m_elements[index];

	weight = element.weight;
	return element.hypothesisPtr;
}


void CHypothesesSet::SetHypothesis(int index, const imeas::IDataSequence* hypothesisPtr, double weight, bool isOwned)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_elements.size()));

	Element& element = m_elements[index];

	element.weight = weight;
	element.hypothesisPtr = hypothesisPtr;
	element.isOwned = isOwned;
}


const imeas::IDataSequence* CHypothesesSet::PopHypothesis(int index, double& weight, bool& isOwned)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_elements.size()));

	Element& element = m_elements[index];

	const imeas::IDataSequence* retVal = element.hypothesisPtr;
	weight = element.weight;
	isOwned = element.isOwned;

	element.weight = 0;
	element.hypothesisPtr = NULL;
	element.isOwned = false;

	return retVal;
}


int CHypothesesSet::GetBestHypothesisIndex() const
{
	if (!m_isBestKnown){
		double bestWeight = -1;
		m_bestHypothesisIndex = -1;

		int hypothesesCount = int(m_elements.size());
		for (int i = 0; i < hypothesesCount; ++i){
			double weight = m_elements[i].weight;

			if (weight > bestWeight){
				bestWeight = weight;
				m_bestHypothesisIndex = i;
			}
		}

		m_isBestKnown = true;
	}

	return m_bestHypothesisIndex;
}


} // namespace iorn


