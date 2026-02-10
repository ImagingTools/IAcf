// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-IAcf-Commercial
#ifndef iocv_CCorrSearchProcessorComp_included
#define iocv_CCorrSearchProcessorComp_included


// ACF includes
#include <iimg/IBitmap.h>
#include <iproc/TSyncProcessorCompBase.h>

// ACF-Solutions includes
#include <iipr/IFeaturesConsumer.h>
#include <iipr/IImageToFeatureProcessor.h>
#include <iipr/ISearchParams.h>
#include <iipr/ISearchConstraints.h>


namespace iocv
{


/**
	Gray scale correlation processor using simple OpenCV correlation engine.
*/
class CCorrSearchProcessorComp:
			public iproc::TSyncProcessorCompBase<iipr::IImageToFeatureProcessor>,
			virtual public iipr::ISearchConstraints
{
public:
	typedef iproc::TSyncProcessorCompBase<iipr::IImageToFeatureProcessor> BaseClass;

	I_BEGIN_COMPONENT(CCorrSearchProcessorComp);
		I_REGISTER_INTERFACE(iipr::ISearchConstraints);
		I_ASSIGN(m_searchParamsIdAttrPtr, "SearchParamsId", "ID of search parameters", false, "SearchParamsId");
		I_ASSIGN(m_defaultSearchParamsCompPtr, "DefaultSearchParams", "Default search parameters, if not taken from parameter set", false, "DefaultSearchParams");
		I_ASSIGN(m_aoiParamIdAttrPtr, "AoiParamId", "ID of rectangle area of interest", true, "AoiParamId");
		I_ASSIGN(m_defaultAoiCompPtr, "DefaultAoi", "Default AOI, if not taken from parameter set", false, "DefaultAoi");
		I_ASSIGN(m_modelParamIdAttrPtr, "ModelParamId", "ID of search model image in parameters set", true, "ModelParamId");
		I_ASSIGN(m_defaultModelImageCompPtr, "DefaultModelImage", "Default search model image, if not taken from parameter set", false, "DefaultModelImage");
	I_END_COMPONENT;

	// reimplemented (iipr::IImageToFeatureProcessor)
	virtual iproc::IProcessor::TaskState DoExtractFeatures(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap& image,
				iipr::IFeaturesConsumer& results,
				ibase::IProgressManager* progressManagerPtr = NULL) override;

	// reimplemented (iproc::IProcessor)
	virtual iproc::IProcessor::TaskState DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL) override;

	// reimplemented (iipr::ISearchConstraints)
	virtual int GetSearchSupportedFlags() const override;
	virtual const istd::CRange& GetRotationRangeConstraints() const override;
	virtual const istd::CRange& GetScaleRangeConstraints() const override;
	virtual const istd::CIntRange& GetResultsCountConstraints() const override;

protected:
	/**
		Returns -1 if there was an error during processing, or number of models found if search succeeded.
	*/
	virtual int DoModelSearch(
				const iimg::IBitmap& image,
				const iimg::IBitmap& modelImage,
				const i2d::CRect& aoi,
				double minScore,
				iipr::IFeaturesConsumer& result);

private:
	I_ATTR(QByteArray, m_searchParamsIdAttrPtr);
	I_REF(iipr::ISearchParams, m_defaultSearchParamsCompPtr);
	I_ATTR(QByteArray, m_aoiParamIdAttrPtr);
	I_REF(i2d::IObject2d, m_defaultAoiCompPtr);
	I_ATTR(QByteArray, m_modelParamIdAttrPtr);
	I_REF(iimg::IBitmap, m_defaultModelImageCompPtr);
};


} // namespace iocv


#endif // !iocv_CCorrSearchProcessorComp_included


