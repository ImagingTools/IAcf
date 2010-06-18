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
				const IFeatureInfo* infoPtr,
				double weight,
				double position,
				int edgeMode);

	int GetEdgeMode() const;

	// reimplemented (iipr::IFeature)
	virtual const IFeatureInfo* GetFeatureInfo() const;
	virtual double GetWeight() const;
	virtual imath::CVarVector GetValue() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	const IFeatureInfo* m_infoPtr;

	double m_weight;
	imath::CVarVector m_value;
	int m_edgeMode;
};


} // namespace iipr


#endif // !iipr_CCaliperFeature_included


