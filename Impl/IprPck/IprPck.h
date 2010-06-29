#ifndef IprPck_included
#define IprPck_included


// ACF includes
#include "imod/TModelWrap.h"
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iipr/TMorphoProcessorComp.h"
#include "iipr/CMovingAverageProcessorComp.h"
#include "iipr/CMultidimensionalFilterParamsComp.h"
#include "iipr/CProcessedAcquisitionComp.h"
#include "iipr/CLocalDifferenceProcessorComp.h"
#include "iipr/CLocalDifferenceFilterParams.h"
#include "iipr/TUnaryTransformProcessorComp.h"
#include "iipr/TGaussianProcessorComp.h"
#include "iipr/CGaussian2dFilterParams.h"
#include "iipr/CCaliperParamsComp.h"
#include "iipr/CExtremumCaliperProcessorComp.h"
#include "iipr/CLineProjectionProcessorComp.h"
#include "iipr/CCircleFindProcessorComp.h"
#include "iipr/CPositionFromImageSupplierComp.h"
#include "iipr/CRectDerivativeProcessorComp.h"
#include "iipr/CFeaturesContainer.h"
#include "iipr/CHeaviestFeatureConsumer.h"
#include "iipr/CFeatureToImageMapperProcessorComp.h"
#include "iipr/CSearchBasedFeaturesSupplierComp.h"
#include "iipr/CLineProjectionSupplierComp.h"
#include "iipr/CImageHistogramProcessorComp.h"


/**
	system-undependent image processing package.
*/
namespace IprPck
{


typedef iipr::CProcessedAcquisitionComp ProcessedCamera;
typedef iipr::CMovingAverageProcessorComp MovingAverageProcessor;
typedef icomp::TModelCompWrap<iipr::CMultidimensionalFilterParamsComp> RectangularFilterParams;
typedef iipr::CLocalDifferenceProcessorComp LocalDifferenceProcessor;	
typedef icomp::TMakeComponentWrap<imod::TModelWrap<iipr::CLocalDifferenceFilterParams>, iser::ISerializable> LocalDifferenceFilterParams;
typedef iipr::CMorphoMaxOperatorComp MorphoMaxOperator;
typedef iipr::CMorphoMinOperatorComp MorphoMinOperator;
typedef iipr::CInvertOperatorComp InvertOperator;
typedef iipr::CGaussianFilterComp GaussianFilter;
typedef icomp::TMakeComponentWrap<imod::TModelWrap<iipr::CGaussian2dFilterParams>, iser::ISerializable> GaussianFilterParams;
typedef icomp::TModelCompWrap<iipr::CCaliperParamsComp> CaliperParams;
typedef iipr::CExtremumCaliperProcessorComp ExtremumCaliperProcessor;
typedef iipr::CLineProjectionProcessorComp LineProjectionProcessor;
typedef iipr::CCircleFindProcessorComp CircleFindProcessor;
typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<iipr::CFeaturesContainer>,
			iipr::IFeaturesConsumer,
			iipr::IFeaturesContainer,
			iser::ISerializable,
			istd::IChangeable,
			imod::IModel> FeaturesContainer;
typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<iipr::CHeaviestFeatureConsumer>,
			iipr::IFeaturesConsumer,
			iipr::IFeaturesContainer,
			iser::ISerializable,
			istd::IChangeable,
			imod::IModel> HeaviestFeatureConsumer;
typedef iipr::CFeatureToImageMapperProcessorComp FeatureToImageMapperProcessor;
typedef icomp::TModelCompWrap<iipr::CPositionFromImageSupplierComp> PositionFromImageSupplier;
typedef icomp::TModelCompWrap<iipr::CSearchBasedFeaturesSupplierComp> SearchBasedFeaturesSupplier;
typedef icomp::TModelCompWrap<iipr::CLineProjectionSupplierComp> ProjectionSupplier;

typedef iipr::CRectDerivativeProcessorComp RectDerivativeProcessor;
typedef iipr::CImageHistogramProcessorComp ImageHistogramProcessor;


} // namespace IprPck


#endif // !IprPck_included


