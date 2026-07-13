if(NOT DEFINED ACFSLNDIR)
	set(ACFSLNDIR "$ENV{ACFSLNDIR}")
endif()

include(${ACFSLNDIR}/Config/CMake/AcfSlnEnv.cmake)

if(NOT DEFINED IACFDIR)
	set(IACFDIR "$ENV{IACFDIR}")
endif()

if(NOT DEFINED IACFDIR_BUILD)
	if(DEFINED ENV{IACFDIR_BUILD})
		set(IACFDIR_BUILD "$ENV{IACFDIR_BUILD}")
	else()
		set(IACFDIR_BUILD ${IACFDIR})
	endif()
endif()

if(NOT DEFINED OPENCVDIR_4_5_3)
	set(OPENCVDIR_4_5_3 "${ACFPUBLICDIR}/3rdParty/OpenCV/4.5.3")
endif()

# AuxInclude for generated files — always needed (build-tree-specific, not
# carried by imported targets).
include_directories("${IACFDIR_BUILD}/AuxInclude/${TARGETNAME}")

# Legacy mode: global include/link dirs for repos that haven't migrated to
# find_package(IAcf) + target-based deps yet. Skipped when ACF_MODERN_CMAKE is ON.
if(NOT ACF_MODERN_CMAKE)
	include_directories("${IACFDIR}/Include")
	include_directories("${IACFDIR}/Impl")
	link_directories(${IACFDIR_BUILD}/Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME})
endif()

message(VERBOSE "IAcf link_directories ${IACFDIR_BUILD}/Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME}")



