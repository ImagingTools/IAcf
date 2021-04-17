project(iocv LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)

message("OPENCVDIR_3_3_1 " "$ENV{OPENCVDIR_3_3_1}")

include_directories("$ENV{OPENCVDIR_3_3_1}/include")

qt5_use_modules(${PROJECT_NAME} Widgets Gui)
