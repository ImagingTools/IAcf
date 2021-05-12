include_directories("$ENV{OPENCVDIR_3_3_1}/include")

if(WIN32)
	if(CMAKE_BUILD_TYPE EQUAL "Relase")
	target_link_libraries(${PROJECT_NAME} opencv_highgui331 opencv_core331 opencv_features2d331 opencv_objdetect331 opencv_imgproc331 opencv_videoio331 opencv_imgcodecs331 opencv_calib3d331 opencv_photo331)
    else()
        target_link_libraries(${PROJECT_NAME} opencv_highgui331d opencv_core331d opencv_features2d331d opencv_objdetect331d opencv_imgproc331d opencv_videoio331d opencv_imgcodecs331d opencv_calib3d331d opencv_photo331d)
	endif()
endif()

if(UNIX)
#unix
    target_link_libraries(${PROJECT_NAME} opencv_highgui opencv_core opencv_features2d opencv_objdetect opencv_imgproc opencv_videoio opencv_imgcodecs opencv_calib3d opencv_photo331)
endif()
