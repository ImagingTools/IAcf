include_directories("$ENV{OPENCVDIR_4_5_3}/include")

if(WIN32)
	if(CMAKE_BUILD_TYPE EQUAL "Relase")
        target_link_libraries(${PROJECT_NAME} opencv_world453)
    else()
        target_link_libraries(${PROJECT_NAME} opencv_world453d)
    endif()
endif()

if(UNIX)
#unix
    target_link_libraries(${PROJECT_NAME} opencv_world453)
endif()

