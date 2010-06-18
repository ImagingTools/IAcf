#ifndef iipr_CSearchFeature_included
#define iipr_CSearchFeature_included


#include "i2d/CVector2d.h"

#include "iipr/IFeature.h"


namespace iipr
{


/**
	Implementation of IFeature interface for pattern search features.
*/
class CSearchFeature: virtual public iipr::IFeature
{
public:
	CSearchFeature(
				const IFeatureInfo* infoPtr,
				const i2d::CVector2d& position,
				const i2d::CVector2d& scale,
				double angle, double score);

	virtual double GetAngle() const;
	virtual i2d::CVector2d GetScale() const;

	// reimplemented (iipr::IFeature)
	virtual const IFeatureInfo* GetFeatureInfo() const;
	virtual double GetWeight() const;
	virtual imath::CVarVector GetValue() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	const IFeatureInfo* m_infoPtr;

	i2d::CVector2d m_value;
	i2d::CVector2d m_scale;
	double m_angle;
	double m_score;
};


} // namespace iipr


#endif // !iipr_CSearchFeature_included

