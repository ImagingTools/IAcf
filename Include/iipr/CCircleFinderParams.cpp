#include "iipr/CCircleFinderParams.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


#include "istd/TChangeNotifier.h"


namespace iipr
{


CCircleFinderParams::CCircleFinderParams()
:	m_isOutlierEliminationEnabled(false),
	m_minOutlierDistance(5),
	m_raysCount(-1)
{
}


// reimplemented (iipr::ICircleFinderParams)

bool CCircleFinderParams::IsOutlierEliminationEnabled() const
{
	return m_isOutlierEliminationEnabled;
}


void CCircleFinderParams::SetOutlierEliminationEnabled(bool isOutlierEliminationEnabled)
{
	if (isOutlierEliminationEnabled != m_isOutlierEliminationEnabled){
		istd::CChangeNotifier notifier(this);

		m_isOutlierEliminationEnabled = isOutlierEliminationEnabled;
	}
}


double CCircleFinderParams::GetMinOutlierDistance() const
{
	return m_minOutlierDistance;
}


void CCircleFinderParams::SetMinOutlierDistance(double minOutlierDistance)
{
	if (minOutlierDistance != m_minOutlierDistance){
		istd::CChangeNotifier notifier(this);

		m_minOutlierDistance = minOutlierDistance;
	}
}


int CCircleFinderParams::GetRaysCount() const
{
	return m_raysCount;
}


void CCircleFinderParams::SetRaysCount(int raysCount)
{
	if (raysCount != m_raysCount){
		istd::CChangeNotifier notifier(this);

		m_raysCount = raysCount;
	}
}


// reimplemented (iser::ISerializable)

bool CCircleFinderParams::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag isOutlierEliminationEnabledTag("OutlierEliminationEnabled", "Outliers elimination is enabled");
	retVal = retVal && archive.BeginTag(isOutlierEliminationEnabledTag);
	retVal = retVal && archive.Process(m_isOutlierEliminationEnabled);
	retVal = retVal && archive.EndTag(isOutlierEliminationEnabledTag);

	static iser::CArchiveTag minOutlierDistanceTag("MinOutlierDistance", "Minimal outliers distance");
	retVal = retVal && archive.BeginTag(minOutlierDistanceTag);
	retVal = retVal && archive.Process(m_minOutlierDistance);
	retVal = retVal && archive.EndTag(minOutlierDistanceTag);

	static iser::CArchiveTag raysCountTag("RaysCount", "Number of projection rays");
	retVal = retVal && archive.BeginTag(raysCountTag);
	retVal = retVal && archive.Process(m_raysCount);
	retVal = retVal && archive.EndTag(raysCountTag);

	return retVal;
}


} // namespace iipr

