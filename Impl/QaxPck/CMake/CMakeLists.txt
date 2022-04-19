project(QaxPck LANGUAGES CXX)

#include(${ACFDIR}/Config/CMake/QtBaseConfig.cmake)
include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)



#qt5_use_modules(${PROJECT_NAME} Axcontainer Multimedia )

target_link_directories(${PROJECT_NAME} PUBLIC $ACFDIR/Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME})
target_link_libraries(${PROJECT_NAME} imm)

#target_link_directories(${PROJECT_NAME} PUBLIC ../../../Lib/${TARGETNAME})
target_link_libraries(${PROJECT_NAME} iqaxmm)

find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS AxContainer REQUIRED)
target_link_libraries(${PROJECT_NAME}  Qt${QT_VERSION_MAJOR}::AxContainer)

