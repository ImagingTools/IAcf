project(QwtPck LANGUAGES CXX)

link_directories($ENV{QWTDIR_6_1_2}/Lib/${TARGETNAME})
link_directories($ENV{QWT3DDIR_0_3_1}/Lib/${TARGETNAME})

include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)
include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(../../../Config/CMake/Qwt.cmake)
include(../../../Config/CMake/Qwt3d.cmake)

link_directories(../../../Lib/${TARGETNAME})
target_link_libraries(${PROJECT_NAME} iqwt iqwt3d)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} opengl32 glu32)
endif()

qt5_use_modules(${PROJECT_NAME} Core Widgets OpenGL Gui)

