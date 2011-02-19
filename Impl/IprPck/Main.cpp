#include "icomp/export.h"

#include "IprPck.h"


namespace IprPck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE(
			"IprPck",
			"Image processing package",
			"\"Image Processing\"" IM_PROJECT("IACF") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			RectImageSmoothProcessor,
			"Fast smooth image processor using simple rectangle kernel",
			"Image Processing Rectangle Kernel");

I_EXPORT_COMPONENT(
			MultidimensionalFilterParams,
			"Rectangular filter parameter",
			"Filter Image Processing Parameter");

I_EXPORT_COMPONENT(
			LocalDifferenceProcessor,
			"Line difference image processor",
			"Image Processing");

I_EXPORT_COMPONENT(
			ProcessedCamera,
			"Processed Camera",
			"Image Bitmap Acquisition Snap Parameters Camera Processing");

I_EXPORT_COMPONENT(
			MorphoMinOperator,
			"Morphological minimum operator with rectangular kernel",
			"Morphological Operator Image Processing");

I_EXPORT_COMPONENT(
			MorphoMaxOperator,
			"Morphological maximum operator with rectangular kernel",
			"Morphological Operator Image Processing");

I_EXPORT_COMPONENT(
			InvertOperator,
			"Invertation operator",
			"Operator Image Processing");

I_EXPORT_COMPONENT(
			GaussianFilter,
			"Gaussian filter",
			"Filter Operator Gaussian Image Processing");

I_EXPORT_COMPONENT(
			GaussianFilterParams,
			"Gaussian filter params",
			"Filter Operator Gaussian Image Processing");

I_EXPORT_COMPONENT(
			CaliperParams,
			"Parameters for caliper tool",
			"Caliper Weight Direction Gradients Projection Parameters");

I_EXPORT_COMPONENT(
			ExtremumCaliperProcessor,
			"Caliper implementation extracting projection extremums",
			"Caliper Extremum Minimum Maximum Projection Processor");

I_EXPORT_COMPONENT(
			LineProjectionProcessor,
			"Line projection implementation for single line width",
			"Projection Line Processor");

I_EXPORT_COMPONENT(
			CircleFindProcessor,
			"Circle finder processor",
			"Circle Finder Annulus Caliper Processor");

I_EXPORT_COMPONENT(
			FeaturesContainer,
			"Container of feature set used to store processing results of some processors",
			"Model Caliper Search Feature Extraction Container");

I_EXPORT_COMPONENT(
			FeatureToImageMapperProcessor,
			"Processor converting feature container to set of positions using mapper of features",
			"Feature Position Processor Mapper Caliper Convertion");

I_EXPORT_COMPONENT(
			PositionFromImageSupplier,
			"Supplier implementation returning position found using caliper tool",
			"Model Caliper Position Processing Supplier");

I_EXPORT_COMPONENT(
			SearchBasedFeaturesSupplier,
			"Supplier implementation returning set of geometrical features found by pattern search",
			"Model Pattern Search Processing Supplier");

I_EXPORT_COMPONENT(
			ProjectionSupplier,
			"Supplier implementation returning image projection data",
			"Supplier Projection Data Image Bitmap Model");

I_EXPORT_COMPONENT(
			RectDerivativeProcessor,
			"Calculate derivative of projection using rectangular kernel",
			"Projection Derivative Processor Filter Rectangular Kernel");

I_EXPORT_COMPONENT(
			ImageHistogramProcessor,
			"Calculate image histogram",
			"Processor Histogram");


I_EXPORT_COMPONENT(
			CircleFinderParams,
			"Circle finder parameters",
			"Circle Finder Processor Parameters");

} // namespace IprPck


