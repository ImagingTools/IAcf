project(WinPck LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)

link_directories(../../../Lib/${TARGETNAME})
target_link_libraries(${PROJECT_NAME} iwin)
