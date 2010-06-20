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
				double weight,
				const i2d::CVector2d& position,
				double angle,
				const i2d::CVector2d& scale);

	const i2d::CVector2d& GetPosition() const;
	double GetAngle() const;
	const i2d::CVector2d& GetScale() const;

	// reimplemented (iipr::IFeature)
	virtual double GetWeight() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	double m_weight;
	i2d::CVector2d m_position;
	double m_angle;
	i2d::CVector2d m_scale;
};


} // namespace iipr


#endif // !iipr_CSearchFeature_included

