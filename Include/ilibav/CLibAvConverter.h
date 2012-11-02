#ifndef ilibav_CLibAvConverter_included
#define ilibav_CLibAvConverter_included


#define __STDC_CONSTANT_MACROS

// LIBAV includes
extern "C"{
#include "libavformat/avformat.h"
#undef PixelFormat
#undef BYTES_PER_SAMPLE
#undef BITS_PER_SAMPLE
#undef WAVE_FORMAT_PCM
}

// ACF includes
#include "istd/CIndex2d.h"
#include "iimg/IBitmap.h"


namespace ilibav
{


/**
	Helper class containing static methods to convert between AvLib and ACF classes.
*/
class CLibAvConverter
{
public:
	static bool ConvertBitmap(const AVFrame& avFrame, const istd::CIndex2d& size, AVPixelFormat pixelFormat, iimg::IBitmap& result);	

protected:
	struct Rgba{
		quint8 r;
		quint8 g;
		quint8 b;
		quint8 a;
	};
};


} // namespace ilibav


#endif // !ilibav_CLibAvConverter_included


