#ifndef iorn_CHypothesisInfo_included
#define iorn_CHypothesisInfo_included


// ACF includes
#include "istd/IChangeable.h"
#include "istd/CRange.h"


namespace iorn
{


/**
	Data sequence information for single processing hypothesis.
	Data sequence for hypothesis contains on active channels and optional single weight channel.
*/
class CHypothesisInfo: virtual public istd::IChangeable
{
public:
	enum ChannelType
	{
		CT_NORMAL,
		CT_WEIGHT
	};

	CHypothesisInfo(int samplesCount, int activeChannelsCount, bool isWeightChannel);

	int GetActiveChannelsCount() const;
	int GetWeightChannelIndex() const;

	virtual int GetSamplesCount() const;
	virtual int GetChannelsCount() const;
	virtual istd::CRange GetValueRange(int channelIndex = -1) const;
	virtual bool HasChannelType(int channelType) const;
	virtual int GetChannelType(int channelIndex) const;

private:
	int m_samplesCount;
	int m_activeChannelsCount;
	bool m_isWeightChannel;
};


} // namespace iorn


#endif // !iorn_CHypothesisInfo_included


