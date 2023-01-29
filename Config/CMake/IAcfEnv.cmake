if(NOT DEFINED ACFSLNDIR)
	set(ACFSLNDIR "$ENV{ACFSLNDIR}")
endif()

include(${ACFSLNDIR}/Config/CMake/AcfSlnEnv.cmake)

if(NOT DEFINED IACFDIR)
	set(IACFDIR "$ENV{IACFDIR}")
endif()


if(NOT DEFINED OPENCVDIR_4_5_3)
	set(OPENCVDIR_4_5_3 "${ACFPUBLICDIR}/3rdParty/OpenCV/4.5.3")
endif()

include_directories("${IACFDIR}/AuxInclude/${TARGETNAME}")
include_directories("${IACFDIR}/Include")
include_directories("${IACFDIR}/Impl")

link_directories(${IACFDIR}/Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME})



