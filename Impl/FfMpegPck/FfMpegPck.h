// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-IAcf-Commercial
#ifndef FfMpegPck_included
#define FfMpegPck_included


#include <icomp/TModelCompWrap.h>

#include <ilibav/CLibAvVideoDecoderComp.h>


/**
	Package based on FFMPEG library.
*/
namespace FfMpegPck
{


typedef icomp::TModelCompWrap<ilibav::CLibAvVideoDecoderComp> LibAvVideoDecoder;


} // namespace FfMpegPck


#endif // !FfMpegPck_included


