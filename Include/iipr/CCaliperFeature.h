#ifndef iipr_CCaliperFeature_included
#define iipr_CCaliperFeature_included


#include "iipr/IFeature.h"


namespace iipr
{


/**
	Implementation of IFeature interface for caliper-extracted features.
*/
class CCaliperFeature: virtual public IFeature
{
public:
	enum EdgeMode
	{
		EM_FALLING,
		EM_RISING
	};

	CCaliperFeature(
				double weight,
				double position,
				int edgeMode);

	double GetPosition() const;
	int GetEdgeMode() const;

	// reimplemented (iipr::IFeature)
	virtual double GetWeight() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	double m_weight;
	double m_position;
	int m_edgeMode;
};


} // namespace iipr


#endif // !iipr_CCaliperFeature_included


