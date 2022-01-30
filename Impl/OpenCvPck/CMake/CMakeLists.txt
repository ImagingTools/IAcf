project(OpenCvPck LANGUAGES CXX)

link_directories($ENV{OPENCVDIR_4_5_3}/lib/${CMAKE_BUILD_TYPE}${TARGETNAME})
message("LIB_OPENCVDIR_4_5_3 " $ENV{OPENCVDIR_4_5_3}/lib/${TARGETNAME})

include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)
include(../../../Config/CMake/OpenCV_4_5_3.cmake)

target_link_libraries(${PROJECT_NAME} iblob imeas iipr icalib iedge iinsp )

link_directories(../../../Lib/${TARGETNAME})
target_link_libraries(${PROJECT_NAME} iocv)

qt5_use_modules(${PROJECT_NAME} Widgets Gui)
