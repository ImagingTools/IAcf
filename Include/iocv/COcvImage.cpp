#include <iocv/COcvImage.h>

#include <istd/CChangeNotifier.h>


namespace iocv
{


// public static methods

bool COcvImage::ConvertFromBitmap(const iimg::IBitmap& bitmap, cv::Mat& outputImage)
{
	const istd::CIndex2d imageSize = bitmap.GetImageSize();
	const int lineBytesCount = bitmap.GetLineBytesCount();

	const iimg::IBitmap::PixelFormat pixelFormat = bitmap.GetPixelFormat();
	int valueType = 0;
	switch (pixelFormat)
	{
	case iimg::IBitmap::PF_GRAY:
		valueType = CV_8UC1;
		break;
	case iimg::IBitmap::PF_RGB24:
		valueType = CV_8UC3;
		break;
	case iimg::IBitmap::PF_RGB:
	case iimg::IBitmap::PF_RGBA:
		valueType = CV_8UC4;
		break;
	case iimg::IBitmap::PF_FLOAT32:
		valueType = CV_32FC1;
		break;
	case iimg::IBitmap::PF_FLOAT64:
		valueType = CV_64FC1;
		break;

	default:
		return false;
	}

	outputImage = cv::Mat(imageSize.GetY(), imageSize.GetX(), valueType, lineBytesCount);

	for (int y = 0; y < imageSize.GetY(); ++y){
		const quint8* inputBitmapLinePtr = (const quint8*)bitmap.GetLinePtr(y);
		quint8* outputBitmapLinePtr = (quint8*)outputImage.ptr(y);

		std::memcpy(outputBitmapLinePtr, inputBitmapLinePtr, lineBytesCount);
	}

	return true;
}


bool COcvImage::ConvertToBitmap(const cv::Mat& image, iimg::IBitmap& outputBitmap)
{
	iimg::IBitmap::PixelFormat pixelFormat;

	switch (image.type()){
	case CV_8UC1:
		pixelFormat = PF_GRAY;
		break;

	case CV_16UC1:
		pixelFormat = PF_GRAY16;
		break;

	case CV_32SC1:
		pixelFormat = PF_GRAY32;
		break;

	case CV_8UC3:
		pixelFormat = PF_RGB24;
		break;

	case CV_8UC4:
		pixelFormat = PF_RGB;
		break;

	case CV_32FC1:
		pixelFormat = PF_FLOAT32;
		break;

	case CV_64FC1:
		pixelFormat = PF_FLOAT32;
		break;

	default:
		return false;
	}

	istd::CChangeNotifier notifier(&outputBitmap);
	Q_UNUSED(notifier);

	if (!outputBitmap.CreateBitmap(pixelFormat, istd::CIndex2d(image.cols, image.rows))){
		return false;
	}

	for (int y = 0; y < image.rows; ++y){
		quint8* outputBitmapLinePtr = (quint8*)outputBitmap.GetLinePtr(y);
		const quint8* inputRowPtr = image.data + y * image.step;

		std::memcpy(outputBitmapLinePtr, inputRowPtr, image.step);
	}

	return true;
}


} // namespace iocv


