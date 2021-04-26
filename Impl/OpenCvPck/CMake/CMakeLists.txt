project(OpenCvPck LANGUAGES CXX)

link_directories($ENV{OPENCVDIR_3_3_1}/lib/${CMAKE_BUILD_TYPE}${TARGETNAME})
#link_directories($ENV{OPENCVDIR_3_3_1}/lib/${TARGETNAME})
message("LIB_OPENCVDIR_3_3_1 " $ENV{OPENCVDIR_3_3_1}/lib/${TARGETNAME})

include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)
include(../../../Config/CMake/OpenCV_3_3_1.cmake)

target_link_libraries(${PROJECT_NAME} iblob imeas iipr icalib iedge iinsp )

link_directories(../../../Lib/${TARGETNAME})
target_link_libraries(${PROJECT_NAME} iocv)

qt5_use_modules(${PROJECT_NAME} Widgets Gui)
