project(OpenCvPck LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)
include(${ACFDIR}/Config/CMake/AcfStdGui.cmake)
include(../../../Config/CMake/OpenCV_4_5_3.cmake)

acf_use_qt_graphics_modules()

target_link_libraries(${PROJECT_NAME} iblob imeas iipr icalib iedge iinsp )
target_link_libraries(${PROJECT_NAME} iocv)
