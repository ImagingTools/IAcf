project(iqwt3d LANGUAGES CXX)

link_directories($ENV{QWT3DDIR_0_3_1}/Lib/${TARGETNAME})

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)
include(../../../Config/CMake/Qwt3d.cmake)

qt5_use_modules(${PROJECT_NAME} Core Widgets OpenGL Gui)
