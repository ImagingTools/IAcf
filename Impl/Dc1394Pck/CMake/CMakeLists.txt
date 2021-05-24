project(Dc1394Pck LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/QtBaseConfig.cmake)
include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)

include_directories("$ENV{LIBDC1394_2_2_0}")
target_link_directories(${PROJECT_NAME} PUBLIC ${COMPILER_DIR}/Lib/${TARGETNAME})
target_link_libraries(${PROJECT_NAME} -lidc1394)

if(APPLE)
	target_link_libraries(${PROJECT_NAME} -licam)
	target_link_libraries($ENV{LIBDC1394_2_2_0} -lidc1394)
endif()
