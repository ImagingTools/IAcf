#ifndef iocv_CHoughBasedSearchProcessorComp_included
#define iocv_CHoughBasedSearchProcessorComp_included


// ACF includes
#include <i2d/CCircle.h>
#include <iimg/IBitmap.h>
#include <iproc/TSyncProcessorCompBase.h>

// ACF-Solutions includes
#include <iipr/IFeaturesConsumer.h>
#include <iipr/IImageToFeatureProcessor.h>
#include <iipr/ISearchParams.h>
#include <iipr/ISearchConstraints.h>
#include <iipr/TWeightedFeatureWrap.h>


namespace iocv
{


/**
	Search of patterns based on hough transformation.
	Three processing modes are supported:
	* Search of lines
	* Search of circles
	* Generalized search of any shapes based on model template given by image
*/
class CHoughBasedSearchProcessorComp:
			public iproc::TSyncProcessorCompBase<iipr::IImageToFeatureProcessor>,
			virtual public iipr::ISearchConstraints
{
public:
	typedef iproc::TSyncProcessorCompBase<iipr::IImageToFeatureProcessor> BaseClass;

	enum ModelType
	{
		MT_CIRCLE,
		MT_LINE,
		MT_TEMPLATE
	};

	I_BEGIN_COMPONENT(CHoughBasedSearchProcessorComp);
		I_REGISTER_INTERFACE(iipr::ISearchConstraints);
		I_ASSIGN(m_aoiParamIdAttrPtr, "AoiParamId", "ID of rectangle area of interest", true, "Region");
		I_ASSIGN(m_defaultAoiCompPtr, "DefaultAoi", "Default AOI, if not taken from parameter set", false, "DefaultAoi");
		I_ASSIGN(m_scaleParamIdAttrPtr, "ScaleParamId", "ID of scale parameter in the parameter set", true, "Scale");
		I_ASSIGN(m_defaultScaleCompPtr, "DefaultScale", "Default scale parameter, if not taken from parameter set", false, "DefaultScale");
		I_ASSIGN(m_minDistanceParamIdAttrPtr, "MinDistanceParamId", "ID of minimal distance parameter in the parameter set", true, "MinDistance");
		I_ASSIGN(m_defaultMinDistanceCompPtr, "DefaultMinDistance", "Default minimal distance parameter, if not taken from parameter set", false, "DefaultMinDistance");
		I_ASSIGN(m_edgeThresholdParamIdAttrPtr, "EdgeThresholdParamId", "ID of edge threshold parameter in the parameter set", true, "EdgeThreshold");
		I_ASSIGN(m_defaultEdgeThresholdCompPtr, "DefaultEdgeThreshold", "Default edge threshold parameter, if not taken from parameter set", false, "DefaultEdgeThreshold");
		I_ASSIGN(m_houghThresholdParamIdAttrPtr, "HoughThresholdParamId", "ID of hough threshold parameter in the parameter set", true, "HoughThreshold");
		I_ASSIGN(m_defaultHoughThresholdCompPtr, "DefaultHoughThreshold", "Default hough threshold parameter, if not taken from parameter set", false, "DefaultHoughThreshold");
		I_ASSIGN(m_radiusRangeParamIdAttrPtr, "RadiusRangeParamId", "ID of radius range parameter (used for circle search only) in the parameter set", true, "RadiusRange");
		I_ASSIGN(m_defaultRadiusRangeCompPtr, "DefaultRadiusRange", "Default radius range parameter (used for circle search only), if not taken from parameter set", false, "DefaultRadiusRange");
		I_ASSIGN(m_searchModelTypeAttrPtr, "SearchModelType", "Type of the search model.\n0 - circle\n1 - line\n2 - general model template", true, MT_CIRCLE);
		I_ASSIGN(m_modelImagePathAttrPtr, "ModelImagePathId", "ID of model image path parameter (used for template search only) in the parameter set", true, "ModelImagePath");
	I_END_COMPONENT;

	// reimplemented (iipr::IImageToFeatureProcessor)
	virtual int DoExtractFeatures(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap& image,
				iipr::IFeaturesConsumer& results,
				ibase::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (iipr::ISearchConstraints)
	virtual int GetSearchSupportedFlags() const;
	virtual const istd::CRange& GetRotationRangeConstraints() const;
	virtual const istd::CRange& GetScaleRangeConstraints() const;
	virtual const istd::CIntRange& GetResultsCountConstraints() const;

protected:
	/**
		Returns -1 if there was an error during processing, or number of models found if search succeeded.
	*/
	virtual int DoCircleSearch(
				const iimg::IBitmap& image,
				const i2d::CRect& aoi,
				int scale,
				int minDistance,
				int edgeThreshold,
				int houghThreshold,
				int minRadius,
				int maxRadius,
				iipr::IFeaturesConsumer& result);
	/**
		Returns -1 if there was an error during processing, or number of models found if search succeeded.
	*/
	virtual int DoLineSearch(
				const iimg::IBitmap& image,
				const i2d::CRect& aoi,
				int scale,
				int minDistance,
				int edgeThreshold,
				int houghThreshold,
				iipr::IFeaturesConsumer& result);
	/**
		Returns -1 if there was an error during processing, or number of models found if search succeeded.
	*/
	virtual int DoTemplateSearch(
			const iimg::IBitmap& image,
			const iimg::IBitmap& modelImage,
			const i2d::CRect& aoi,
			const istd::CRange& scaleRange,
			double scaleStep,
			const istd::CRange& angleRange,
			double angleStep,
			int minDistance,
			int levels,
			const istd::CRange& cannyEdgeThreshold,
			iipr::IFeaturesConsumer& result);

private:
	typedef iipr::TWeightedFeatureWrap<i2d::CCircle> CircleFeature;

	I_ATTR(QByteArray, m_aoiParamIdAttrPtr);
	I_REF(i2d::IObject2d, m_defaultAoiCompPtr);

	I_ATTR(QByteArray, m_scaleParamIdAttrPtr);
	I_REF(imeas::INumericValue, m_defaultScaleCompPtr);

	I_ATTR(QByteArray, m_minDistanceParamIdAttrPtr);
	I_REF(imeas::INumericValue, m_defaultMinDistanceCompPtr);

	I_ATTR(QByteArray, m_edgeThresholdParamIdAttrPtr);
	I_REF(imeas::INumericValue, m_defaultEdgeThresholdCompPtr);

	I_ATTR(QByteArray, m_houghThresholdParamIdAttrPtr);
	I_REF(imeas::INumericValue, m_defaultHoughThresholdCompPtr);

	I_ATTR(QByteArray, m_radiusRangeParamIdAttrPtr);
	I_REF(imeas::INumericValue, m_defaultRadiusRangeCompPtr);

	I_ATTR(QByteArray, m_modelImagePathAttrPtr);

	I_ATTR(int, m_searchModelTypeAttrPtr); 
};


} // namespace iocv


#endif // !iocv_CHoughBasedSearchProcessorComp_included


