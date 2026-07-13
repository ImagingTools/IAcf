if (USE_FIND_PACKAGE)
	find_package(ZLIB)
	target_link_libraries(${PROJECT_NAME} ${ACF_LIBRARY_LINK_SCOPE} ZLIB::ZLIB)
else()

	include_directories(${ZLIBDIR}/include)

	if(WIN32)
		target_link_libraries(${PROJECT_NAME} ${ACF_LIBRARY_LINK_SCOPE} ${ZLIBDIR}/lib/${CMAKE_BUILD_TYPE}_${COMPILER_CODE}/zlib.lib)
	endif()

	if(UNIX)
		target_link_libraries(${PROJECT_NAME} ${ACF_LIBRARY_LINK_SCOPE} -lz)
	endif()

endif()
