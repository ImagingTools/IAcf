include_directories(${ZLIBDIR}/include)

target_link_libraries(${PROJECT_NAME} ${ZLIBDIR}/lib/${CMAKE_BUILD_TYPE}${TARGETNAME}/zlib.lib)
