project(iqaxmm LANGUAGES CXX)


include(${ACFDIR}/Config/CMake/StaticConfig.cmake)


include_directories("${Qt5_DIR}/../../../include/ActiveQt")

qt5_use_modules(${PROJECT_NAME} Axcontainer Multimedia )
