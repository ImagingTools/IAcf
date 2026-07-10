project(FfMpegPck LANGUAGES CXX)


include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)

include_directories($ENV{FFMPEGDIR_20121010}/include)
target_link_directories(${PROJECT_NAME} PUBLIC $ENV{FFMPEGDIR_20121010}/lib)

target_link_libraries(${PROJECT_NAME} ${ACF_PACKAGE_LINK_SCOPE}
	ilibav
	swscale
	avdevice
	avformat
	avcodec
	avutil
	avfilter
	postproc
	swresample
)

