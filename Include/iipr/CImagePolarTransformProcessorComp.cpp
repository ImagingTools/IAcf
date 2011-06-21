#include "iipr/CImagePolarTransformProcessorComp.h"


 // ACF includes
#include "istd/TChangeNotifier.h"

#include "ibase/CSize.h"

#include "iimg/CBitmapRegion.h"


// IACF includes
#include "iipr/TImagePixelInterpolator.h"


namespace iipr
{


// reimplemented (iproc::IProcessor)

int CImagePolarTransformProcessorComp::DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* /*progressManagerPtr*/)
{
	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		return TS_INVALID;
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (outputBitmapPtr == NULL){
		return TS_INVALID;
	}

	const i2d::IObject2d* aoiPtr = NULL;
	if (paramsPtr != NULL && m_aoiParamsIdAttrPtr.IsValid()){
		aoiPtr = dynamic_cast<const i2d::IObject2d*>(paramsPtr->GetParameter((*m_aoiParamsIdAttrPtr).ToString()));
	}

	return ConvertImage(*inputBitmapPtr, aoiPtr, *outputBitmapPtr) ? TS_OK : TS_INVALID;
}


// private methods

bool CImagePolarTransformProcessorComp::ConvertImage(
			const iimg::IBitmap& input,
			const i2d::IObject2d* aoiPtr,
			iimg::IBitmap& outputBitmap) const
{
	if (input.IsEmpty()){
		return true;
	}

	const i2d::IObject2d* realAoiPtr = aoiPtr;
	i2d::CRectangle imageRect(input.GetImageSize());
	if (aoiPtr == NULL){
		realAoiPtr = &imageRect;
	}

	iimg::CBitmapRegion bitmapRegion(&input);
	if (!bitmapRegion.CreateFromGeometry(*realAoiPtr)){
		return false;
	}

	if (bitmapRegion.IsBitmapRegionEmpty()){
		SendWarningMessage(0, "Cannot process an empty region");
		
		return false;
	}

	int radiusScale = 4;
	int angleScale = 4;

	i2d::CRectangle regionRect = bitmapRegion.GetBoundingBox();
	i2d::CVector2d aoiCenter = regionRect.GetCenter();
	i2d::CVector2d diffVector = aoiCenter - regionRect.GetTopLeft();
	int radius = int(::ceil(diffVector.GetLength())) * radiusScale;
	int angleRange = angleScale * 360;

	int r1 = 0;
	int r2 = radius;

	const i2d::CAnnulus* annulusPtr = dynamic_cast<const i2d::CAnnulus*>(realAoiPtr);
	if (annulusPtr != NULL){
		r1 = int(::ceil(annulusPtr->GetInnerRadius())) * radiusScale;
		r2 = int(::floor(annulusPtr->GetOuterRadius())) * radiusScale;
		radius = r2 - r1;
	}

	if (!outputBitmap.CreateBitmap(input.GetPixelFormat(), istd::CIndex2d(angleRange, radius))){
		return false;
	}

	iipr::TImagePixelInterpolator<I_BYTE> pixelInterpolator(input, iipr::IImageInterpolationParams::IM_NO_INTERPOLATION);
	int pixelComponentsCount = input.GetComponentsCount();

	for (int componentIndex = 0; componentIndex < pixelComponentsCount; componentIndex++){
		for (int r = 0; r < radius; r++){
			I_BYTE* outputImageBeginPtr = (I_BYTE*)outputBitmap.GetLinePtr(r);
		
			for (int alpha = 0; alpha < angleRange; alpha++){
				double radian = imath::GetRadianFromDegree(alpha / angleScale);
		
				double x = (r + r1) * cos(radian) / radiusScale; 			
				double y = (r  + r1) * sin(radian) / radiusScale;

				x += aoiCenter.GetX();
				y += aoiCenter.GetY();

				*(outputImageBeginPtr +  alpha * pixelComponentsCount + componentIndex) = pixelInterpolator.GetInterpolatedValue(x, y, componentIndex);
			}
		}
	}

	return true;
}

/*
I_BYTE CImagePolarTransformProcessorComp::GetInterpolatedValue(const iimg::IBitmap& bitmap, double x, double y) const
{
	int cx = int(::ceil(x));
	int cy = int(::ceil(y));

	int ix = int(::floor(x));
	int iy = int(::floor(y));

	double dx = cx - x;
	double dy = cy - y;

	double v1 = (1.0 - dx) * (1.0 - dy) * GetImageValue(bitmap, ix , iy);
	double v2 = (dx) * (1.0 - dy) * GetImageValue(bitmap, cx , iy);
	double v3 = (dy) * (1.0 - dx) * GetImageValue(bitmap, ix, cy);
	double v4 = (dy) * (dx) * GetImageValue(bitmap, cx, cy);

	int retVal = int(v1 + v2 + v3 + v4 + 0.5);
	if (retVal > 255){
		retVal = 255;
	}

	return I_BYTE(retVal);
}


I_BYTE CImagePolarTransformProcessorComp::GetImageValue(const iimg::IBitmap& bitmap, int x, int y) const
{
	I_BYTE* imageBufferPtr = (I_BYTE*)bitmap.GetLinePtr(0);
	int imageLineDifference = bitmap.GetLinesDifference();
	int inputImageHeight = bitmap.GetImageSize().GetY();
	int inputImageWidth = bitmap.GetImageSize().GetX();

	if (x < 0){
		x = 0;
	}

	if (x >= inputImageWidth){
		x = inputImageWidth - 1;
	}

	if (y < 0){
		y = 0;
	}

	if (y >= inputImageHeight){
		y = inputImageHeight - 1;
	}

	return *(imageBufferPtr + x + imageLineDifference * y);
}
*/

} // namespace iipr


