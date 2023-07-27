project(iqwt3d LANGUAGES CXX)

find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS OpenGL REQUIRED)

link_directories($ENV{QWT3DDIR_0_3_2}/Lib/${TARGETNAME})

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)
include(../../../Config/CMake/Qwt3d.cmake)

if(QT_VERSION_MAJOR EQUAL 6)
  find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS OpenGLWidgets REQUIRED)
  target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
endif()

acf_use_qt_graphics_modules()
