project(FfMpegPck LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/QtBaseConfig.cmake)
include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)


include_directories("$ENV{FFMPEGDIR_20121010}/include" "$ENV{FFMPEGDIR_20121010}/msinttypes")
include_directories("$ENV{OPENCVDIR_4_5_3}/include")

target_link_directories(${PROJECT_NAME} PUBLIC $ENV{FFMPEGDIR_20121010}/lib/${TARGETNAME})

target_link_libraries(${PROJECT_NAME} -licam -licam)
target_link_libraries(${PROJECT_NAME} -lilibav)
target_link_libraries(${PROJECT_NAME} -lswscale -lavdevice -lavformat -lavcodec -lavutil)
