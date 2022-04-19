project(OpenCvPck LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)
include(../../../Config/CMake/OpenCV_4_5_3.cmake)

target_link_libraries(${PROJECT_NAME} iblob imeas iipr icalib iedge iinsp )
target_link_libraries(${PROJECT_NAME} iocv)
