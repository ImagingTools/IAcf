project(iqwt3d LANGUAGES CXX)

find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS OpenGL OpenGLWidgets REQUIRED)

link_directories($ENV{QWT3DDIR_0_3_2}/Lib/${TARGETNAME})

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)
include(../../../Config/CMake/Qwt3d.cmake)

target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

#qt5_use_modules(${PROJECT_NAME} Core Widgets OpenGL Gui)
