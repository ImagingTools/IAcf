#ifndef iipr_CRectImageSmoothProcessorComp_included
#define iipr_CRectImageSmoothProcessorComp_included


#include "iipr/IMultidimensionalFilterParams.h"
#include "iipr/TImageProcessorCompBase.h"


namespace iipr
{
	

/**
	Implementation of moving average operator.
*/
class CRectImageSmoothProcessorComp: public iipr::TImageProcessorCompBase<IMultidimensionalFilterParams>
{
public:
	typedef iipr::TImageProcessorCompBase<IMultidimensionalFilterParams> BaseClass;

protected:		
	// reimplemented (iipr::TImageProcessorCompBase<iipr::IMultidimensionalFilterParams>)
	virtual bool ProcessImage(
				const IMultidimensionalFilterParams* paramsPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);
};


} // namespace iipr


#endif // !iipr_CRectImageSmoothProcessorComp_included

