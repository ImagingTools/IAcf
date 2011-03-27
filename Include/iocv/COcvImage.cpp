#include "iocv/COcvImage.h"


namespace iocv
{


// public methods

COcvImage::COcvImage(const IplImage& image)
{
	ConvertToBitmap(image, *this);
}


// public static methods

bool COcvImage::ConvertToBitmap(const IplImage& image, iimg::IBitmap& outputBitmap)
{
	int bitsPerPixel = image.depth * image.nChannels;
	
	switch (image.nChannels){
		case 1:
			return !outputBitmap.CreateBitmap(
						istd::CIndex2d(image.width, image.height),
						image.imageData,
						true,
						image.widthStep,
						bitsPerPixel, 
						1);
		case 3:
			if (outputBitmap.CreateBitmap(istd::CIndex2d(image.width, image.height), 32, 4)){
				I_BYTE* outputBitmapLinePtr = (I_BYTE*)outputBitmap.GetLinePtr(0);
				std::memset(outputBitmapLinePtr, 255, outputBitmap.GetLineBytesCount() * outputBitmap.GetImageSize().GetY());

				for (int y = 0; y < image.height; y++){
					I_BYTE* outputBitmapLinePtr = (I_BYTE*)outputBitmap.GetLinePtr(y);
					I_BYTE* inputBitmapLinePtr = (I_BYTE*)image.imageData + y * image.widthStep;

					for (int x = 0; x < outputBitmap.GetLineBytesCount(); x += 4){
						int ix = (x >> 2) * 3;

						std::memcpy(outputBitmapLinePtr + x,  inputBitmapLinePtr + ix, 3);
					}
				}

				return true;
			}
			break;
		default:
			I_CRITICAL();
	}

	return false;	
}


} // namespace iocv


