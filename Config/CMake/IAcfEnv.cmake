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

if(NOT ACF_MODERN_CMAKE)
	# Legacy mode: global include/link dirs for repos that haven't migrated to
	# find_package(IAcf) + target-based deps yet. Skipped when ACF_MODERN_CMAKE is ON.
	include_directories("${IACFDIR}/Include")
	include_directories("${IACFDIR}/Impl")
	link_directories(${IACFDIR_BUILD}/Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME})

	message(VERBOSE "IAcf link_directories ${IACFDIR_BUILD}/Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME}")
elseif(NOT TARGET AcfSln::iproc)
	if(NOT DEFINED ACFSLNDIR_BUILD)
		set(ACFSLNDIR_BUILD "${ACFSLNDIR}")
	endif()

	# Discover the Acf and AcfSln packages published by their build trees.
	# In a composite build (Acf, AcfSln and IAcf in the same CMake tree) their alias
	# targets are already visible - skip find_package to avoid requiring the
	# not-yet-generated *Targets.cmake export files.
	set(AcfSln_DIR "${ACFSLNDIR_BUILD}/Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME}/cmake")
	message(VERBOSE "IAcf find_package(AcfSln) from ${AcfSln_DIR}")

	find_package(AcfSln REQUIRED GLOBAL)
endif()

