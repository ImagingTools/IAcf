project(FfMpegPck LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)


include_directories($ENV{FFMPEGDIR_20121010}/include)

target_link_directories(${PROJECT_NAME} PUBLIC $ENV{FFMPEGDIR_20121010}/lib)

target_link_libraries(${PROJECT_NAME} icam)
target_link_libraries(${PROJECT_NAME} ilibav)
target_link_libraries(${PROJECT_NAME} swscale avdevice avformat avcodec avutil avfilter postproc swresample)
