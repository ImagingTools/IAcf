include_directories("$ENV{QWT3DDIR_0_3_2}/include")

target_link_libraries(${PROJECT_NAME} ${ACF_LIBRARY_LINK_SCOPE} $ENV{QWT3DDIR_0_3_2}/lib/${CMAKE_BUILD_TYPE}_${TARGETNAME}/qwtplot3d.lib)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} ${ACF_LIBRARY_LINK_SCOPE} opengl32 glu32)
endif()

