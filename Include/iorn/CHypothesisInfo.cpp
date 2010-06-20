#include "iorn/CHypothesisInfo.h"


namespace iorn
{


CHypothesisInfo::CHypothesisInfo(int samplesCount, int activeChannelsCount, bool isWeightChannel)
:	m_samplesCount(samplesCount),
	m_activeChannelsCount(activeChannelsCount),
	m_isWeightChannel(isWeightChannel)
{
}


int CHypothesisInfo::GetActiveChannelsCount() const
{
	return m_activeChannelsCount;
}


int CHypothesisInfo::GetWeightChannelIndex() const
{
	return m_isWeightChannel? m_activeChannelsCount: -1;
}


int CHypothesisInfo::GetSamplesCount() const
{
	return m_samplesCount;
}


int CHypothesisInfo::GetChannelsCount() const
{
	return m_isWeightChannel? m_activeChannelsCount + 1: m_activeChannelsCount;
}


istd::CRange CHypothesisInfo::GetValueRange(int /*channelIndex*/) const
{
	return istd::CRange(0, 1);
}


bool CHypothesisInfo::HasChannelType(int channelType) const
{
	if (channelType == CT_NORMAL){
		return true;
	}
	else if (channelType == CT_WEIGHT){
		return m_isWeightChannel;
	}
	else{
		return false;
	}
}


int CHypothesisInfo::GetChannelType(int channelIndex) const
{
	if (m_isWeightChannel && (channelIndex == m_activeChannelsCount)){
		return CT_WEIGHT;
	}
	else{
		return CT_NORMAL;
	}
}


} // namespace iorn


