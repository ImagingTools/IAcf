/********************************************************************************
**
**	Copyright (C) 2007-2011 Witold Gantzke & Kirill Lepskiy
**
**	This file is part of the IACF Toolkit.
**
**	This file may be used under the terms of the GNU Lesser
**	General Public License version 2.1 as published by the Free Software
**	Foundation and appearing in the file LicenseLGPL.txt included in the
**	packaging of this file.  Please review the following information to
**	ensure the GNU Lesser General Public License version 2.1 requirements
**	will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**	If you are unsure which license is appropriate for your use, please
**	contact us at info@imagingtools.de.
**
** 	See http://www.ilena.org, write info@imagingtools.de or contact
**	by Skype to ACF_infoline for further information about the IACF.
**
********************************************************************************/


#include "ilibav/CLibAvConverter.h"


// LIBAV includes
extern "C"{
#define inline _inline
#include "libswscale/swscale.h"
#undef inline
}

// ACF includes
#include "istd/TChangeNotifier.h"


namespace ilibav
{


// static methods

bool CLibAvConverter::ConvertBitmap(
			const AVFrame& avFrame,
			const istd::CIndex2d& size,
			AVPixelFormat pixelFormat,
			iimg::IBitmap& result)
{
	istd::CChangeNotifier notifier(&result);

	if (result.CreateBitmap(iimg::IBitmap::PF_RGB, size)){
		SwsContext* scaleContextPtr = sws_getContext(
					size.GetX(),
					size.GetY(),
					pixelFormat,
					size.GetX(),
					size.GetY(),
					PIX_FMT_RGB32,
					SWS_FAST_BILINEAR,
					NULL,
					NULL,
					NULL);
		uint8_t* resultData[4] = {(uint8_t*)result.GetLinePtr(0), 0, 0, 0};
		int resultLinesize[4] = {result.GetLineBytesCount(), 0, 0, 0};
		sws_scale(	scaleContextPtr,
					const_cast<AVFrame&>(avFrame).data,
					const_cast<AVFrame&>(avFrame).linesize,
					0,
					size.GetY(),
					resultData,
					resultLinesize);
		sws_freeContext(scaleContextPtr);

		for (int y = 0; y < size[1]; ++y){
			Rgba* linePtr = (Rgba*)result.GetLinePtr(y);
			for (int x = 0; x < size[0]; ++x){
				Rgba& color = linePtr[x];
				color.a = 255;
			}
		}

		return true;
	}

	return false;
}

} // namespace ilibav


