#include "iipr/CCaliperFeature.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


CCaliperFeature::CCaliperFeature(
			const IFeatureInfo* infoPtr,
			double weight,
			double position,
			int edgeMode)
:	m_infoPtr(infoPtr),
	m_weight(weight),
	m_value(1, position),
	m_edgeMode(edgeMode)
{
}


int CCaliperFeature::GetEdgeMode() const
{
	return m_edgeMode;
}


// reimplemented (iipr::IFeature)

const IFeatureInfo* CCaliperFeature::GetFeatureInfo() const
{
	return m_infoPtr;
}


double CCaliperFeature::GetWeight() const
{
	return m_weight;
}


imath::CVarVector CCaliperFeature::GetValue() const
{
	return m_value;
}


// reimplemented (iser::ISerializable)

bool CCaliperFeature::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag weightTag("Weight", "Weight of feature");
	retVal = retVal && archive.BeginTag(weightTag);
	retVal = retVal && archive.Process(m_weight);
	retVal = retVal && archive.EndTag(weightTag);

	static iser::CArchiveTag valueTag("Value", "Position of caliper feature");
	retVal = retVal && archive.BeginTag(valueTag);
	retVal = retVal && archive.Process(m_value[0]);
	retVal = retVal && archive.EndTag(valueTag);

	return retVal;
}


} // namespace iipr

