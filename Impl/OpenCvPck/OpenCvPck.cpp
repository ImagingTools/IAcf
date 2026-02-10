// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-IAcf-Commercial
#include "OpenCvPck.h"


#include <icomp/export.h>


namespace OpenCvPck
{


I_EXPORT_PACKAGE(
			"Iacf/OpenCv",
			"OpenCV integration",
			"OpenCV" IM_PROJECT("IACF") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			OpenCvAcquisition,
			"Implementation of camera interface using OpenCV API",
			"Camera Acquisition Bitmap Image" IM_TAG("Camera"));

I_EXPORT_COMPONENT(
			OpenCvVideoController,
			"OpenCV-based video controller",
			"Video Multimedia Controller" IM_TAG("Multimedia"));

I_EXPORT_COMPONENT(
			OpenCvBlobExtractor,
			"OpenCV-based blob extraction",
			"Blob Search Object");

I_EXPORT_COMPONENT(
			OpenCvCorrSearchProcessor,
			"OpenCV-based normalized correlation search processor",
			"Normalized Correlation Search Processor Model Image Find Pattern Matching");

I_EXPORT_COMPONENT(
			OpenCvMedianProcessor,
			"OpenCV-based median blur processor",
			"Median Blue Smoothing Processor");

I_EXPORT_COMPONENT(
			OpenCvHoughBasedSearchProcessor,
			"OpenCV-based search processor based on hough transformation",
			"Search Circle Line Processor Feature Hough");

I_EXPORT_COMPONENT(
			OpenCvResamplingProcessor,
			"OpenCV-based resampling processor",
			"Size Resampling Interpolation Processor Transformation");

I_EXPORT_COMPONENT(
			OpenCvEdgeExtractor,
			"OpenCV-based edge extractor",
			"Edge Contour Extractor Object Model");

I_EXPORT_COMPONENT(
			OpenCvAdaptiveBinarization,
			"OpenCV-based adaptive binaraization",
			"Threshold Binarization Segmentation");

I_EXPORT_COMPONENT(
			OpenCvMorphologicalProcessor, 
			"Morphological processor based on OpenCV",
			"Morphology Filter");

I_EXPORT_COMPONENT(
			OpenCvPointGridExtractor,
			"Points grid extractor",
			"Points Extractor" IM_TAG("Processor"));

I_EXPORT_COMPONENT(
			OpenCvIntrinsicCameraCalibration,
			"Intrinsic camera calibration",
			"Calibration");

I_EXPORT_COMPONENT(
			OpenCvIntrinsicCameraCalibrationSupplier,
			"Calculates intrinsic camera calibration",
			"Camera Calibration Distortion" IM_TAG("Supplier"));

I_EXPORT_COMPONENT(
			OpenCvUndistortImageProcessor,
			"Makes undistorted image with camera calibration",
			"Undistort Image" IM_TAG("Processor"));

I_EXPORT_COMPONENT(
			OpenCvDenoisingProcessor,
			"Non-local Means Denoising processor",
			"Denoising Processor");


} // namespace OpenCvPck


