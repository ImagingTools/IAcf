// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-IAcf-Commercial
#ifndef iocv_COcvSubdiv2d_included
#define iocv_COcvSubdiv2d_included


// STL includes
#include <vector>

// OpenCV includes
#include <opencv2/opencv.hpp>


namespace iocv
{


class COcvSubdiv2d: public cv::Subdiv2D
{
public:
	struct Triangle
	{
		Triangle()
			:a(-1),
			b(-1),
			c(-1)
		{
		}
	
		int a;
		int b;
		int c;
	};

	void GetTrianglesIndices(std::vector<Triangle>& triangleList) const;
};


} // namespace iocv


#endif // !iocv_COcvSubdiv2d_included


