#ifndef iipr_CExtremumCaliperProcessor_included
#define iipr_CExtremumCaliperProcessor_included


// ACF includes
#include "iproc/TSyncProcessorWrap.h"

#include "iipr/ICaliperProcessor.h"
#include "iipr/IFeatureInfo.h"


namespace iipr
{


/**
	Calculate extremum of projection data (iipr::CProjectionData).
	Using this tool together with derivative calculation it is possible to create caliper tool (extremums of first derivative).
*/
class CExtremumCaliperProcessor:
			public iproc::TSyncProcessorWrap<ICaliperProcessor>,
			protected IFeatureInfo
{
public:
	typedef iproc::TSyncProcessorWrap<ICaliperProcessor> BaseClass;

	/**
		Do extremum features analyze.
	*/
	virtual bool DoExtremumCaliper(const CProjectionData& source, const ICaliperParams& params, IFeaturesConsumer& results);

	/**
		Get parameter ID used to extract caliper parameter object from parameter set.
	*/
	const std::string& GetCaliperParamsId() const;

	/**
		Set parameter ID used to extract caliper parameter object from parameter set.
		It is only needed while using general processing interface iproc::IProcessor.
	*/
	void SetCaliperParamsId(const std::string& id);

	// reimplemented (iipr::ICaliperProcessor)
	virtual bool DoCaliper(
				const CProjectionData& projection,
				const ICaliperParams* paramsPtr,
				IFeaturesConsumer& results);

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr);

protected:
	// reimplemented (iipr::IFeatureInfo)
	virtual int GetFeatureTypeId() const;
	virtual const istd::CString& GetFeatureTypeDescription() const;
	virtual istd::CString GetFeatureDescription(const IFeature& feature) const;

private:
	std::string m_caliperParamsId;
};


// inline methods

inline const std::string& CExtremumCaliperProcessor::GetCaliperParamsId() const
{
	return m_caliperParamsId;
}


inline void CExtremumCaliperProcessor::SetCaliperParamsId(const std::string& id)
{
	m_caliperParamsId = id;
}


} // namespace iipr


#endif // !iipr_CExtremumCaliperProcessor_included


