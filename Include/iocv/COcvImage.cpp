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


template <class PixelType>
bool ConvertGrayBitmap(const cv::Mat& image, iimg::IBitmap::PixelFormat pixelFormat, iimg::IBitmap& outputBitmap)
{
	if (outputBitmap.CreateBitmap(pixelFormat, istd::CIndex2d(image.cols, image.rows))) {
		for (int y = 0; y < image.rows; ++y) {
			const PixelType* inputBitmapLinePtr = (const PixelType*)image.ptr(y);
			PixelType* outputBitmapLinePtr = (PixelType*)outputBitmap.GetLinePtr(y);

			std::memcpy(outputBitmapLinePtr, inputBitmapLinePtr, sizeof(PixelType) * image.cols);
		}

		return true;
	}
	
	return false;
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
				quint8* outputBitmapPtr = (quint8*)outputBitmap.GetLinePtr(0);
				std::memset(outputBitmapPtr, 255, outputBitmap.GetLineBytesCount() * outputBitmap.GetImageSize().GetY());

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
	switch (image.type()) {
	case CV_32FC1:
		return ConvertGrayBitmap<float>(image, iimg::IBitmap::PF_FLOAT32, outputBitmap);

	case CV_64FC1:
		return ConvertGrayBitmap<double>(image, iimg::IBitmap::PF_FLOAT64, outputBitmap);

	default:
		break;
	}

	const IplImage& iplImage = IplImage(image);

	return ConvertToBitmap(iplImage, outputBitmap);
}


} // namespace iocv


