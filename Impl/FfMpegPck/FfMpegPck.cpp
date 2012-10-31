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


#include "FfMpegPck.h"


#include "icomp/export.h"


namespace FfMpegPck
{


I_EXPORT_PACKAGE(
			"Iacf/FfMpeg",
			"Package based on FFMPEG library",
			"ACF Solutions MPEG Video Audio FFMPEG AvLib" IM_PROJECT("IACF") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			LibAvVideoDecoder,
			"Video decoder based on libAv library",
			"Video Decoder Controller LibAv Bitmap Frame Grab AVI MPEG" IM_TAG("Multimedia"));

I_EXPORT_COMPONENT(
			LibAvRtspStreamingCamera,
			"Video camera for rtsp stream with H264 codec based on liv555 library (stream) and libAv library (video)",
			"Video Decoder Controller LibAv Bitmap Grab Stream RTSP H264" IM_TAG("Multimedia"));


} // namespace FfMpegPck


