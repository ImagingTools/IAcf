include_directories("$ENV{QWTDIR_6_2_0}/src")
message(VERBOSE "Path QWTDIR_6_2_0: " $ENV{QWTDIR_6_2_0}/src)

target_link_libraries(${PROJECT_NAME} ${ACF_LIBRARY_LINK_SCOPE} $ENV{QWTDIR_6_2_0}/lib/${CMAKE_BUILD_TYPE}_${TARGETNAME}/qwt.lib)

