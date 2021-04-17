project(iqwt LANGUAGES CXX)


link_directories($ENV{QWTDIR_6_1_2}/Lib/${TARGETNAME})

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)
include(../../../Config/CMake/Qwt.cmake)

qt5_use_modules(${PROJECT_NAME} Core Widgets Gui)
