#ifndef iipr_CSimpleFeature_included
#define iipr_CSimpleFeature_included


// STL includes
#include <set>
#include <map>

// ACF includes
#include "imath/TVarVector.h"

#include "iipr/IFeature.h"


namespace iipr
{


/**
	Implementation of IFeature interface for pattern search features.
*/
class CSimpleFeature: virtual public IFeature
{
public:
	typedef imath::CVarVector BaseClass;

	CSimpleFeature(double weight);

	void SetValue(int elementId, const imath::CVarVector& value);
	void InsertSlaveFeature(const IFeature* featurePtr);
	void InsertSourceFeature(const IFeature* featurePtr);

	// reimplemented (iipr::IFeature)
	virtual double GetWeight() const;
	virtual const imath::CVarVector& GetValue(int elementId) const;
	virtual const imath::CVarVector& GetLocalValue(int elementId) const;
	virtual Features GetSlaveFeatures() const;
	virtual Features GetSourceFeatures() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	double m_weight;

	typedef std::map<int, imath::CVarVector> ValuesMap;
	ValuesMap m_valuesMap;

	Features m_slaveFeatures;
	Features m_sourceFeatures;
};


} // namespace iipr


#endif // !iipr_CSimpleFeature_included

