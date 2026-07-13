project(Dc1394Pck LANGUAGES CXX)


include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)

include_directories("$ENV{LIBDC1394_2_2_0}")

target_link_libraries(${PROJECT_NAME} ${ACF_PACKAGE_LINK_SCOPE}
	idc1394
)

