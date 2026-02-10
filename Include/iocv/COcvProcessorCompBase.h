// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-IAcf-Commercial
#pragma once


// ACF-Solutions includes
#include <iproc/TSyncProcessorCompBase.h>
#include <iimg/IBitmap.h>
#include <iprm/IParamsSet.h>

// OpenCV includes
#include <opencv2/imgproc/imgproc.hpp>


namespace iocv
{


class COcvProcessorCompBase : public iproc::CSyncProcessorCompBase
{
public:
	typedef iproc::CSyncProcessorCompBase BaseClass;
	
	I_BEGIN_BASE_COMPONENT(COcvProcessorCompBase);
		I_ASSIGN(m_aoiIdAttrPtr, "AoiParamId", "ID of the AOI parameter in the parameter set", false, "AoiParamId");
	I_END_COMPONENT;

protected:
	bool GetBitmaps(const istd::IPolymorphic* inputPtr, 
				istd::IChangeable* outputPtr, 
				const iimg::IBitmap* &inputBitmapPtr,
				iimg::IBitmap* &outputBitmapPtr) const;

	void MakeProcessedRegionMatrices(const iimg::IBitmap* inputBitmapPtr,
				const iimg::IBitmap* outputBitmapPtr,
				const iprm::IParamsSet* paramsPtr,
				cv::Mat& inputMatrix, 
				cv::Mat& outputMatrix) const;

	bool EmplaceProcessedRegionBack(iimg::IBitmap* outputBitmapPtr,
				const iprm::IParamsSet* paramsPtr,
				cv::Mat& outputMatrix) const;

protected:
	I_ATTR(QByteArray, m_aoiIdAttrPtr);
};


} // namespace iocv


