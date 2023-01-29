project(QwtPck LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)
include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../../Config/CMake/Qwt.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../../Config/CMake/Qwt3d.cmake)

target_link_libraries(${PROJECT_NAME} iqwt iqwt3d)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} opengl32 glu32)
endif()

