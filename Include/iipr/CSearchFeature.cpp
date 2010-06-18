#include "iipr/CSearchFeature.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


// public methods

CSearchFeature::CSearchFeature(
			const IFeatureInfo* infoPtr,
			const i2d::CVector2d& position,
			const i2d::CVector2d& scale,
			double angle,
			double score)
:	m_infoPtr(infoPtr),
	m_value(position),
	m_scale(scale),
	m_angle(angle),
	m_score(score)
{
}


double CSearchFeature::GetAngle() const
{
	return m_angle;
}


i2d::CVector2d CSearchFeature::GetScale() const
{
	return m_scale;
}


// reimplemented (iipr::IFeature)

const IFeatureInfo* CSearchFeature::GetFeatureInfo() const
{
	return m_infoPtr;
}


double CSearchFeature::GetWeight() const
{
	return m_score;
}


imath::CVarVector CSearchFeature::GetValue() const
{
	return m_value;
}


// reimplemented (iser::ISerializable)

bool CSearchFeature::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag scoreTag("Score", "Model score");
	retVal = retVal && archive.BeginTag(scoreTag);
	retVal = retVal && archive.Process(m_score);
	retVal = retVal && archive.EndTag(scoreTag);

	static iser::CArchiveTag valueTag("Position", "Position of found model");
	retVal = retVal && archive.BeginTag(valueTag);
	retVal = retVal && m_value.Serialize(archive);
	retVal = retVal && archive.EndTag(valueTag);

	static iser::CArchiveTag angleTag("Angle", "Angle of found model");
	retVal = retVal && archive.BeginTag(angleTag);
	retVal = retVal && archive.Process(m_angle);
	retVal = retVal && archive.EndTag(angleTag);

	static iser::CArchiveTag scaleTag("Scale", "Scale of found model");
	retVal = retVal && archive.BeginTag(scaleTag);
	retVal = retVal && m_scale.Serialize(archive);
	retVal = retVal && archive.EndTag(scaleTag);

	return retVal;
}


} // namespace iipr

