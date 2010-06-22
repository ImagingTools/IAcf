#ifndef iipr_CSearchFeature_included
#define iipr_CSearchFeature_included


#include "i2d/CVector2d.h"
#include "i2d/CPosition2d.h"

#include "iipr/IFeature.h"


namespace iipr
{


/**
	Implementation of IFeature interface for pattern search features.
*/
class CSearchFeature:
			public i2d::CPosition2d,
			virtual public iipr::IFeature
{
public:
	typedef i2d::CPosition2d BaseClass;

	CSearchFeature(
				double weight,
				const i2d::CVector2d& position,
				double angle,
				const i2d::CVector2d& scale);

	double GetAngle() const;
	const i2d::CVector2d& GetScale() const;

	// reimplemented (iipr::IFeature)
	virtual double GetWeight() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	double m_weight;
	double m_angle;
	i2d::CVector2d m_scale;
};


} // namespace iipr


#endif // !iipr_CSearchFeature_included

