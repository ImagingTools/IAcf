project(iqwt LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)
include(../../../Config/CMake/Qwt.cmake)

acf_use_qt_graphics_modules()
