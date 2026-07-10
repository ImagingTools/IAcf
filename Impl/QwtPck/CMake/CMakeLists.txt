project(QwtPck LANGUAGES CXX)


include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../../Config/CMake/Qwt.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../../Config/CMake/Qwt3d.cmake)

target_link_libraries(${PROJECT_NAME} ${ACF_PACKAGE_LINK_SCOPE}
	iqwt
	iqwt3d
)

