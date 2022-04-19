project(iocv LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)

message("OPENCVDIR_4_5_3 " "$ENV{OPENCVDIR_4_5_3}")

include_directories("$ENV{OPENCVDIR_4_5_3}/include")
