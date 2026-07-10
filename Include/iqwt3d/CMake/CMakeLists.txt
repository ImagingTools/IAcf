project(iqwt3d LANGUAGES CXX)

s
find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS OpenGL REQUIRED)

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)
include(../../../Config/CMake/Qwt3d.cmake)

if(QT_VERSION_MAJOR EQUAL 6)
	find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS OpenGLWidgets REQUIRED)
	target_link_libraries(${PROJECT_NAME} ${ACF_LIBRARY_LINK_SCOPE} Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
endif()

