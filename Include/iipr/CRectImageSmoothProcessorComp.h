#ifndef iipr_CRectImageSmoothProcessorComp_included
#define iipr_CRectImageSmoothProcessorComp_included


// ACF-Solutions includes
#include "imeas/IUnitInfo.h"

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
			virtual public IMultidimensionalFilterConstraints,
			virtual protected imeas::IUnitInfo
{
public:
	typedef iipr::TImageParamProcessorCompBase<IMultidimensionalFilterParams> BaseClass;

	enum UnitMode
	{
		UM_PIXEL,
		UM_PERCENT,
		UM_PERCENT_DIAG
	};

	enum BorderMode
	{
		BM_STRETCH_KERNEL,
		BM_REDUCE_OUTPUT
	};

	I_BEGIN_COMPONENT(CRectImageSmoothProcessorComp);
		I_REGISTER_INTERFACE(IMultidimensionalFilterConstraints);
		I_ASSIGN(m_unitModeAttrPtr, "UnitMode", "Define used units:\n\t0 - Pixels\n\t1 - Percent of width and height\n\t2 - Percent of diagonal", true, 0);
		I_ASSIGN(m_borderModeAttrPtr, "BorderMode", "Define used mode for border area:\n\t0 - Kernel will be stretched, original image size will be outputed\n\t1 - Border area removed, output image will be smaller", true, 0);
	I_END_COMPONENT;

	// reimplemented (iipr::IMultidimensionalFilterConstraints)
	virtual int GetFilterDimensionsCount() const;
	virtual istd::CString GetFilterDescription(int dimension) const;
	virtual const imeas::IUnitInfo& GetFilterUnitInfo(int dimension) const;

protected:
	// reimplemented (iipr::TImageParamProcessorCompBase<iipr::IMultidimensionalFilterParams>)
	virtual bool ParamProcessImage(
				const IMultidimensionalFilterParams* paramsPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);

	// reimplemented (imeas::IUnitInfo)
	virtual int GetUnitType() const;
	virtual istd::CString GetUnitName() const;
	virtual double GetDisplayMultiplicationFactor() const;
	virtual istd::CRange GetValueRange() const;
	virtual const imath::IDoubleManip& GetValueManip() const;

private:
	I_ATTR(int, m_unitModeAttrPtr);
	I_ATTR(int, m_borderModeAttrPtr);
};


} // namespace iipr


#endif // !iipr_CRectImageSmoothProcessorComp_included


