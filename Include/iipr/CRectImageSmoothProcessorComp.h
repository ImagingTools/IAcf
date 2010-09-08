#ifndef iipr_CRectImageSmoothProcessorComp_included
#define iipr_CRectImageSmoothProcessorComp_included


#include "iipr/IMultidimensionalFilterParams.h"
#include "iipr/IMultidimensionalFilterConstraints.h"
#include "iipr/TImageParamProcessorCompBase.h"


namespace iipr
{


/**
	Implementation of image smooth operator with homogeneous rectangle kernel.
	This implementation uses moving average alghorithm.
*/
class CRectImageSmoothProcessorComp:
			public TImageParamProcessorCompBase<IMultidimensionalFilterParams>,
			virtual public IMultidimensionalFilterConstraints
{
public:
	typedef iipr::TImageParamProcessorCompBase<IMultidimensionalFilterParams> BaseClass;

	I_BEGIN_COMPONENT(CRectImageSmoothProcessorComp);
		I_REGISTER_INTERFACE(IMultidimensionalFilterConstraints);
	I_END_COMPONENT;

	// reimplemented (iipr::IMultidimensionalFilterConstraints)
	virtual int GetFilterDimensionsCount() const;
	virtual istd::CRange GetFilterLengthRange(int dimension) const;
	virtual const imath::IDoubleManip& GetFilterLengthManip(int dimension) const;

protected:
	// reimplemented (iipr::TImageParamProcessorCompBase<iipr::IMultidimensionalFilterParams>)
	virtual bool ParamProcessImage(
				const IMultidimensionalFilterParams* paramsPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);
};


} // namespace iipr


#endif // !iipr_CRectImageSmoothProcessorComp_included


