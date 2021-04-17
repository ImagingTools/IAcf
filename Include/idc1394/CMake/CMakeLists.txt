project(idc1394 LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)

include_directories("${PROJECT_SOURCE_DIR}/../../../../AcfSln/Include")
include_directories("$ENV{LIBDC1394_2_2_0}")
