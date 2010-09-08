#ifndef iipr_CLocalDifferenceProcessorComp_included
#define iipr_CLocalDifferenceProcessorComp_included


// ACF includes
#include "istd/CString.h"
#include "iprm/ILinearAdjustParams.h"

#include "iipr/IMultidimensionalFilterParams.h"
#include "iipr/CImageProcessorCompBase.h"


namespace iipr
{
	

/**
	Implementation of horizontal differential operator.
*/
class CLocalDifferenceProcessorComp:
			public CImageProcessorCompBase
{
public:
	typedef CImageProcessorCompBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CLocalDifferenceProcessorComp)
		I_ASSIGN(m_adjustParamsIdAttrPtr, "AdjustParamsId", "ID of parameters for contrast and brightness in parameter set", false, "AdjustParamsId");
		I_ASSIGN(m_filterParamIdAttrPtr, "FilterParamId", "ID of parameter for filter length in parameter set", false, "FilterParamId");
		I_ASSIGN(m_defaultAdjustParamsCompPtr, "DefaultAdjustParams", "Default adjust parameters, if no parameters are specified", false, "DefaultAdjustParams");
		I_ASSIGN(m_defaultFilterParamCompPtr, "DefaultFilterParam", "Default parameter for filter length, if no parameters are specified", false, "DefaultFilterParam");
	I_END_COMPONENT

protected:
	// reimplemented (iipr::CImageProcessorCompBase)
	virtual bool ProcessImage(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);

private:
	I_ATTR(istd::CString, m_adjustParamsIdAttrPtr);
	I_ATTR(istd::CString, m_filterParamIdAttrPtr);
	I_REF(iprm::ILinearAdjustParams, m_defaultAdjustParamsCompPtr);
	I_REF(IMultidimensionalFilterParams, m_defaultFilterParamCompPtr);
};


} // namespace iipr


#endif // !iipr_CLocalDifferenceProcessorComp_included

