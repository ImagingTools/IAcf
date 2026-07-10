# ---------------------------------------------------------------------------
# Generate and export the "IAcf" CMake package.
#
# This mirrors the ACF / AcfSln package exports (Config/CMake/AcfPackageExport.cmake
# in the Acf module and Config/CMake/AcfSlnPackageExport.cmake in the AcfSln
# module). After this runs, downstream projects (ImtCore) can discover IAcf with
# a single call:
#
#     find_package(IAcf REQUIRED)
#     target_link_libraries(myTarget PUBLIC IAcf::iocv IAcf::iqwt ...)
#
# and inherit include directories and inter-library dependencies transitively.
# The generated config pulls in the underlying ACF and AcfSln packages through
# find_dependency(Acf) / find_dependency(AcfSln).
#
# Two flavours of the package are produced:
#  * a build-tree package (no install step required), written next to the
#    compiled libraries so it matches the existing in-tree build layout, and
#  * an install-tree package for a relocatable `cmake --install` deployment.
#
# The IAcf library targets are collected in the ${ACF_EXPORT_SET} export set,
# which the shared StaticConfig.cmake / acf_register_library() machinery from
# Acf registers each library into (ACF_EXPORT_SET defaults to
# "${ACF_PACKAGE_NAME}Targets", i.e. "IAcfTargets").
#
# Included once, centrally, from Build/CMake/CMakeLists.txt after all library
# targets and their dependencies have been declared.
# ---------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

if(NOT DEFINED ACF_EXPORT_SET)
	set(ACF_EXPORT_SET "IAcfTargets")
endif()

# Package version. project(IAcf) does not set one, so fall back to a default
# that still lets consumers request a version and use find_package version
# checks.
if(NOT DEFINED IAcf_VERSION OR IAcf_VERSION STREQUAL "")
	if(DEFINED PROJECT_VERSION AND NOT PROJECT_VERSION STREQUAL "")
		set(IAcf_VERSION "${PROJECT_VERSION}")
	else()
		set(IAcf_VERSION "1.0.0")
	endif()
endif()

# Build-tree location: alongside the produced libraries, mirroring the existing
# Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME} layout.
set(IACF_LIB_OUTPUT_DIR "${IACFDIR_BUILD}/Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME}")
set(IACF_PACKAGE_BUILD_DIR "${IACF_LIB_OUTPUT_DIR}/cmake")

# Ensure the output directory exists before writing export/config files into it,
# otherwise export()/configure_package_config_file() can fail on a fresh build tree.
file(MAKE_DIRECTORY "${IACF_PACKAGE_BUILD_DIR}")

# --- Build-tree export ------------------------------------------------------
export(EXPORT ${ACF_EXPORT_SET}
	NAMESPACE IAcf::
	FILE "${IACF_PACKAGE_BUILD_DIR}/IAcfTargets.cmake")

# Qt major version this package was built against. Baked into the generated
# IAcfConfig.cmake so consumers resolve the matching Qt imported targets.
set(IACF_QT_VERSION_MAJOR "${QT_VERSION_MAJOR}")

configure_package_config_file(
	"${IACFDIR}/Config/CMake/IAcfConfig.cmake.in"
	"${IACF_PACKAGE_BUILD_DIR}/IAcfConfig.cmake"
	INSTALL_DESTINATION "${IACF_PACKAGE_BUILD_DIR}"
	NO_SET_AND_CHECK_MACRO)

write_basic_package_version_file(
	"${IACF_PACKAGE_BUILD_DIR}/IAcfConfigVersion.cmake"
	VERSION "${IAcf_VERSION}"
	COMPATIBILITY SameMajorVersion)

# Allow find_package(IAcf) to locate the build-tree package directly (e.g. via
# CMAKE_PREFIX_PATH=<build>/Lib/<config>/cmake or IAcf_DIR).
message(STATUS "IAcf: build-tree package written to ${IACF_PACKAGE_BUILD_DIR}")

# --- Install-tree export ----------------------------------------------------
install(EXPORT ${ACF_EXPORT_SET}
	NAMESPACE IAcf::
	DESTINATION "lib/cmake/IAcf"
	FILE "IAcfTargets.cmake")

install(FILES
	"${IACF_PACKAGE_BUILD_DIR}/IAcfConfig.cmake"
	"${IACF_PACKAGE_BUILD_DIR}/IAcfConfigVersion.cmake"
	DESTINATION "lib/cmake/IAcf")
