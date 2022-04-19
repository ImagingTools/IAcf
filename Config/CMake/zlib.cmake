include_directories(${ZLIBDIR}/include)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} ${ZLIBDIR}/lib/${CMAKE_BUILD_TYPE}_${COMPILER_CODE}/zlib.lib)
endif()

if(UNIX)
	target_link_libraries(${PROJECT_NAME} -lz)
endif()

