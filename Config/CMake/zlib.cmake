 if(DEFINED ENV{ZLIBDIR})
		set(ZLIBDIR $ENV{ZLIBDIR})
	else()
		set(ZLIBDIR "${ACFDIR}/../3rdParty/quazip")
	endif()

include_directories(${ZLIBDIR}/include)
#find_library (zlibPath 
#				NAMES zlib.lib
#				PATHS "C:/Work/Rpos/AcfPublic/3rdParty/zlib/lib/DebugVC15_64/"
#				NO_DEFAULT_PATH)
#				#Path ${ZLIBDIR}/lib/${CMAKE_BUILD_TYPE}${TARGETNAME})
#				message("zlibPath: " ${zlibPath})

target_link_libraries(${PROJECT_NAME} ${ZLIBDIR}/lib/${CMAKE_BUILD_TYPE}${TARGETNAME}/zlib.lib)
