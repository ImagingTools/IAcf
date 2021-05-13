#AcfSln

if(NOT DEFINED ACFSLNDIR)
	set(ACFSLNDIR "$ENV{ACFSLNDIR}")
endif()

include(${ACFSLNDIR}/Config/CMake/AcfSlnEnv.cmake)


#Iacf
if(NOT DEFINED IACFDIR)
	set(IACFDIR "$ENV{IACFDIR}")
endif()


if(NOT DEFINED OPENCVDIR_3_3_1)
	set(OPENCVDIR_3_3_1 "${ACFDIR}/../3rdParty/OpenCV/3.3.1")
endif()

include_directories("${IACFDIR}/AuxInclude/Qt${QT_DEFAULT_MAJOR_VERSION}_${TARGETNAME}")
include_directories("${IACFDIR}/Include")
include_directories("${IACFDIR}/Impl")

link_directories(${IACFDIR}/Lib/${CMAKE_BUILD_TYPE}${TARGETNAME})



