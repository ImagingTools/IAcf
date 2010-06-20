#include "iipr/CCaliperFeature.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


CCaliperFeature::CCaliperFeature(
			double weight,
			double position,
			int edgeMode)
:	m_weight(weight),
	m_position(position),
	m_edgeMode(edgeMode)
{
}


double CCaliperFeature::GetPosition() const
{
	return m_position;
}


int CCaliperFeature::GetEdgeMode() const
{
	return m_edgeMode;
}


// reimplemented (iipr::IFeature)

double CCaliperFeature::GetWeight() const
{
	return m_weight;
}


// reimplemented (iser::ISerializable)

bool CCaliperFeature::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag weightTag("Weight", "Weight of feature");
	retVal = retVal && archive.BeginTag(weightTag);
	retVal = retVal && archive.Process(m_weight);
	retVal = retVal && archive.EndTag(weightTag);

	static iser::CArchiveTag valueTag("Position", "Position on projection");
	retVal = retVal && archive.BeginTag(valueTag);
	retVal = retVal && archive.Process(m_position);
	retVal = retVal && archive.EndTag(valueTag);

	static iser::CArchiveTag edgeModeTag("EdgeMode", "Edge mode");
	retVal = retVal && archive.BeginTag(edgeModeTag);
	retVal = retVal && archive.Process(m_edgeMode);
	retVal = retVal && archive.EndTag(edgeModeTag);

	return retVal;
}


} // namespace iipr

