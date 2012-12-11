#ifndef iocv_COcvImage_included
#define iocv_COcvImage_included


// OpenCV includes
#include <opencv/cv.h>
#include <opencv/highgui.h>
#undef min
#undef max

// ACF includes
#include "iimg/CBitmapBase.h"


namespace iocv
{

/**
	Wrapper for Ipl image.
	This implementation can be used for conversion from OpenCV image structure to standard ACF bitmap.
*/
class COcvImage: virtual iimg::CBitmapBase
{
public:
	COcvImage(const IplImage& image);

	/**
		Convert OpenCV image to standard ACF bitmap.
	*/
	static bool ConvertToBitmap(const IplImage& image, iimg::IBitmap& outputBitmap);
};


} // namespace iocv


#endif // !iocv_COcvImage_included


