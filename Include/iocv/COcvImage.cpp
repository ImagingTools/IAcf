#include <iocv/COcvImage.h>


// STL includes
#include <cstring>


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
	switch (image.nChannels){
		case 1:
			if (outputBitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, istd::CIndex2d(image.width, image.height))){
				for (int y = 0; y < image.height; ++y){
					quint8* outputBitmapLinePtr = (quint8*)outputBitmap.GetLinePtr(y);
					quint8* inputBitmapLinePtr = (quint8*)image.imageData + y * image.widthStep;

					for (int x = 0; x < image.width; ++x){
						*outputBitmapLinePtr = *inputBitmapLinePtr;

						++outputBitmapLinePtr, ++inputBitmapLinePtr;
					}
				}

				return true;
			}
			break;

		case 3:
			if (outputBitmap.CreateBitmap(iimg::IBitmap::PF_RGB, istd::CIndex2d(image.width, image.height))){
				quint8* outputBitmapLinePtr = (quint8*)outputBitmap.GetLinePtr(0);
				std::memset(outputBitmapLinePtr, 255, outputBitmap.GetLineBytesCount() * outputBitmap.GetImageSize().GetY());

				for (int y = 0; y < image.height; y++){
					quint8* outputBitmapLinePtr = (quint8*)outputBitmap.GetLinePtr(y);
					quint8* inputBitmapLinePtr = (quint8*)image.imageData + y * image.widthStep;

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


bool COcvImage::ConvertToBitmap(const cv::Mat& image, iimg::IBitmap& outputBitmap)
{
	const IplImage& iplImage = IplImage(image);

	return ConvertToBitmap(iplImage, outputBitmap);
}


} // namespace iocv


