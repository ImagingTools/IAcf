include_directories("$ENV{OPENCVDIR_4_5_3}/include")

if(WIN32)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")
		target_link_libraries(${PROJECT_NAME} ${ACF_LIBRARY_LINK_SCOPE} $ENV{OPENCVDIR_4_5_3}/lib/${CMAKE_BUILD_TYPE}_${COMPILER_CODE}/opencv_world453.lib)
	else()
		target_link_libraries(${PROJECT_NAME} ${ACF_LIBRARY_LINK_SCOPE} $ENV{OPENCVDIR_4_5_3}/lib/${CMAKE_BUILD_TYPE}_${COMPILER_CODE}/opencv_world453d.lib)
	endif()
endif()

if(UNIX)
	target_link_libraries(${PROJECT_NAME} ${ACF_LIBRARY_LINK_SCOPE} opencv_world453)
endif()
