#ifndef iorn_CHypothesisInfo_included
#define iorn_CHypothesisInfo_included


#include "imeas/IDataSequenceInfo.h"


namespace iorn
{


/**
	Data sequence information for single processing hypothesis.
	Data sequence for hypothesis contains on active channels and optional single weight channel.
*/
class CHypothesisInfo: virtual public imeas::IDataSequenceInfo
{
public:
	CHypothesisInfo(int samplesCount, int activeChannelsCount, bool isWeightChannel);

	int GetActiveChannelsCount() const;
	int GetWeightChannelIndex() const;

	// reimplemented (imeas::IDataSequenceInfo)
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


