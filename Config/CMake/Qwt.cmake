include_directories("$ENV{QWTDIR_6_2_0}/src")
message("Path QWTDIR_6_1_2: " $ENV{QWTDIR_6_2_0}/src)

target_link_libraries(${PROJECT_NAME} $ENV{QWTDIR_6_2_0}/lib/${CMAKE_BUILD_TYPE}_${TARGETNAME}/qwt.lib)

