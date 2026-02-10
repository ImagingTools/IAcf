// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-IAcf-Commercial
#ifndef iocv_COcvImage_included
#define iocv_COcvImage_included


// OpenCV includes
#include "opencv2/opencv.hpp"

#undef min
#undef max

// ACF includes
#include <iimg/CBitmapBase.h>


namespace iocv
{


/**
	Wrapper for Ipl image.
	This implementation can be used for conversion from OpenCV image structure to standard ACF bitmap.
*/
class COcvImage: virtual iimg::CBitmapBase
{
public:
	/**
		Convert standard ACF bitmap to OpenCV image.
	*/
	static bool ConvertFromBitmap(const iimg::IBitmap& bitmap, cv::Mat& outputImage);

	/**
		Convert OpenCV image to standard ACF bitmap.
	*/
	static bool ConvertToBitmap(const cv::Mat& image, iimg::IBitmap& outputBitmap);
};


} // namespace iocv


#endif // !iocv_COcvImage_included


