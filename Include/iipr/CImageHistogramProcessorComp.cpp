#include "iipr/CImageHistogramProcessorComp.h"


// STL includes
#include <cmath>


 // ACF includes
#include "istd/TChangeNotifier.h"

#include "ibase/CSize.h"


namespace iipr
{


// reimplemented (iproc::IProcessor)

int CImageHistogramProcessorComp::DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* /*progressManagerPtr*/)
{
	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		return TS_INVALID;
	}

	imeas::IDiscrDataSequence* histogramPtr = dynamic_cast<imeas::IDiscrDataSequence*>(outputPtr);
	if (histogramPtr == NULL){
		return TS_INVALID;
	}

	const i2d::CRectangle* aoiPtr = NULL;
	if (paramsPtr != NULL && m_aoiParamsIdAttrPtr.IsValid()){
		aoiPtr = dynamic_cast<const i2d::CRectangle*>(paramsPtr->GetParameter((*m_aoiParamsIdAttrPtr).ToString()));
	}

	return CalculateHistogramFromBitmap(*inputBitmapPtr, aoiPtr, *histogramPtr) ? TS_OK : TS_INVALID;
}


// private methods

bool CImageHistogramProcessorComp::CalculateHistogramFromBitmap(
			const iimg::IBitmap& input,
			const i2d::CRectangle* aoiPtr,
			imeas::IDiscrDataSequence& histogram) const
{
	if (input.IsEmpty()){
		histogram.ResetSequence();

		return true;
	}

	ibase::CSize inputSize(input.GetImageSize());

	inputSize -= ibase::CSize(1, 1);

	i2d::CRectangle realArea = i2d::CRectangle(inputSize);
	if (aoiPtr != NULL){
		realArea  = aoiPtr->GetIntersection(i2d::CRectangle(inputSize));
	}

	if (realArea.IsEmpty() || !realArea.IsValid()){
		return false;
	}

	int leftArea = int(realArea.GetLeft());
	int topArea = int(realArea.GetTop());
	int bottomArea = int(realArea.GetBottom());
	int rightArea = int(realArea.GetRight());

	int componentsBitCount = input.GetComponentBitsCount();
	if (componentsBitCount != 8){
		SendWarningMessage(0, "Empty region for histogram calculation");

		return false;
	}

	int componentsCount = input.GetComponentsCount();
	int usedColorComponents = componentsCount;

	iimg::IBitmap::PixelFormat pixelFormat = input.GetPixelFormat();
	switch (pixelFormat){
		case iimg::IBitmap::PF_RGB:
			usedColorComponents = 3;
			break;
		case iimg::IBitmap::PF_RGBA:
			usedColorComponents = 4;
			break;
	}

	int histogramSize = 256 * usedColorComponents;

	int pixelCount = (rightArea - leftArea + 1) * (bottomArea - topArea + 1);

	istd::TDelPtr<I_DWORD, istd::ArrayAccessor<I_DWORD> > histogramDataPtr(new I_DWORD[histogramSize]);
	I_DWORD* histogramDataBufferPtr = histogramDataPtr.GetPtr();

	std::memset(histogramDataBufferPtr, 0, histogramSize * sizeof(I_DWORD));

	for (int y = topArea; y <= bottomArea; y++){
		I_BYTE* lineDataBeg = (I_BYTE*)input.GetLinePtr(y) + leftArea * componentsCount;
		I_BYTE* lineDataEnd = (I_BYTE*)input.GetLinePtr(y) + rightArea * componentsCount;

		while (lineDataBeg <= lineDataEnd){
			for (int componentIndex = 0; componentIndex < componentsCount; componentIndex++){
				I_BYTE pixelComponentValue = *lineDataBeg++;

				if (componentIndex < usedColorComponents){
					++histogramDataBufferPtr[componentIndex + pixelComponentValue * usedColorComponents];
				}
			}
		}
	}

	double normFactor = pow(2.0, histogram.GetSampleDepth()) - 1;

	for (int histIndex = 0; histIndex < histogramSize; histIndex++){
		double normHist = histogramDataBufferPtr[histIndex] / double(pixelCount);

		histogramDataBufferPtr[histIndex] = I_DWORD(normHist * normFactor + 0.5);
	}

	istd::CChangeNotifier changePtr(&histogram);

	return histogram.CreateDiscrSequence(histogramSize, histogramDataPtr.PopPtr(), true, 0, 0, sizeof(I_DWORD) * 8, usedColorComponents);
}


} // namespace iipr

