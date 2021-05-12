#AcfSln

if(NOT DEFINED ACFSLNDIR)
	set(ACFSLNDIR "$ENV{ACFSLNDIR}")
endif()

include(${ACFSLNDIR}/Config/CMake/AcfSlnEnv.cmake)


#Iacf
if(NOT DEFINED IACFDIR)
	set(IACFDIR "$ENV{IACFDIR}")
endif()

include_directories("${IACFDIR}/AuxInclude/Qt${QT_DEFAULT_MAJOR_VERSION}_${TARGETNAME}")
include_directories("${IACFDIR}/Include")
include_directories("${IACFDIR}/Impl")



