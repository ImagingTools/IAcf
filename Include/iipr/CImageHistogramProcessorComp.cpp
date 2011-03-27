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
		return false;
	}

	int componentsCount = input.GetComponentsCount();
	int histogramSize = 256 * componentsCount;

	istd::TDelPtr<I_DWORD, istd::ArrayAccessor<I_DWORD> > histogramDataPtr(new I_DWORD[histogramSize]);
	I_DWORD* histogramDataBufferPtr = histogramDataPtr.GetPtr();

	std::memset(histogramDataBufferPtr, 0, histogramSize);

	for (int y = topArea; y <= bottomArea; y++){
		I_BYTE* lineDataBeg = (I_BYTE*)input.GetLinePtr(y) + leftArea * componentsCount;
		I_BYTE* lineDataEnd = (I_BYTE*)input.GetLinePtr(y) + rightArea * componentsCount;

		while (lineDataBeg <= lineDataEnd){
			for (int componentIndex = 0; componentIndex < componentsCount; componentIndex++){
				I_BYTE pixelComponentValue = *lineDataBeg++;

				++histogramDataBufferPtr[pixelComponentValue + (componentIndex * 256)];
			}
		}
	}

	istd::CChangeNotifier changePtr(&histogram);

	return histogram.CreateDiscrSequence(histogramSize, histogramDataPtr.PopPtr(), true, 0, 0, sizeof(I_DWORD) * 8, componentsCount);
}


} // namespace iipr

