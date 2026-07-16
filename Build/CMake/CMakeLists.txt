cmake_minimum_required(VERSION 3.26)

project(IAcf)


set(BUILD_TESTING ON CACHE BOOL "Configures projects for testing")

if(NOT DEFINED IACFDIR)
	set(IACFDIR "${CMAKE_CURRENT_LIST_DIR}/../../../IAcf")
endif()

if(NOT DEFINED QTDIR)
	file(TO_CMAKE_PATH "$ENV{QTDIR}" QTDIR)
endif()

if(NOT DEFINED CMAKE_PREFIX_PATH)
	set(CMAKE_PREFIX_PATH "${QTDIR}")
	message("set CMAKE_PREFIX_PATH " ${CMAKE_PREFIX_PATH})
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core)

set(ACF_QT_COMPONENTS Core Widgets Gui Xml Network Svg Concurrent PrintSupport OpenGL)

if(QT_VERSION_MAJOR EQUAL 5)
	list(APPEND ACF_QT_COMPONENTS XmlPatterns)
endif()

if(QT_VERSION_MAJOR EQUAL 6)
	list(APPEND ACF_QT_COMPONENTS SvgWidgets OpenGLWidgets)
endif()

if(BUILD_TESTING)
	list(APPEND ACF_QT_COMPONENTS Test)
endif()

find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS ${ACF_QT_COMPONENTS} REQUIRED)

# Enable modern target-based architecture:
# find_package(Acf)/find_package(AcfSln) provide Acf::/AcfSln:: imported targets with transitive
# include dirs and link deps. Legacy global include_directories()/link_directories() in the Env
# chain are skipped.
set(ACF_MODERN_CMAKE ON CACHE BOOL "Enable modern target-based architecture")

# Pulls in the Acf/AcfSln/IAcf Env chain: sets TARGETNAME, ACFDIR_BUILD, ACFSLNDIR_BUILD and (when
# ACF_MODERN_CMAKE is OFF) the legacy global include/link dirs.
include(${IACFDIR}/Config/CMake/IAcfEnv.cmake)

if(NOT DEFINED IACFDIR_BUILD)
	set(IACFDIR_BUILD "${IACFDIR}")
endif()

file(TO_CMAKE_PATH "${IACFDIR_BUILD}" IACFDIR_BUILD)

# Collect IAcf libraries in their own export set so they are published as the
# IAcf:: package. Read by StaticConfig.cmake / acf_register_library().
set(ACF_PACKAGE_NAME "IAcf")

if(ACF_MODERN_CMAKE)
	acf_define_link_scope_var(ACF_QT_MODULE_LINK_SCOPE "PRIVATE" "Link scope used by acf_use_qt_base_modules and acf_use_qt_graphics_modules")
	acf_define_link_scope_var(ACF_LIBRARY_LINK_SCOPE "PUBLIC" "Link scope used by IAcf inter-library dependencies")
	acf_define_link_scope_var(ACF_PACKAGE_LINK_SCOPE "PRIVATE" "Link scope used by package (Pck) libraries linking their dependencies")
	acf_define_link_scope_var(ACF_APPLICATION_LINK_SCOPE "PRIVATE" "Link scope used by executables linking their dependencies")
endif()

set(INCLUDE_DIR "${IACFDIR_BUILD}/Include")
set(IMPL_DIR "${IACFDIR_BUILD}/Impl")
set(AUX_INCLUDE_DIR "${IACFDIR_BUILD}/AuxInclude/${TARGETNAME}/GeneratedFiles")

# Include
if(WIN32)
	add_subdirectory("${INCLUDE_DIR}/iocv/CMake" "${AUX_INCLUDE_DIR}/iocv")
	add_subdirectory("${INCLUDE_DIR}/iqwt/CMake" "${AUX_INCLUDE_DIR}/iqwt")
	add_subdirectory("${INCLUDE_DIR}/iqwt3d/CMake" "${AUX_INCLUDE_DIR}/iqwt3d")
	add_subdirectory("${INCLUDE_DIR}/iwin/CMake" "${AUX_INCLUDE_DIR}/iwin")

	# ---------- no longer used, but kept for reference ----------
	#add_subdirectory("${INCLUDE_DIR}/iqaxmm/CMake" "${AUX_INCLUDE_DIR}/iqaxmm")
	#add_subdirectory("${INCLUDE_DIR}/ilibav/CMake" "${AUX_INCLUDE_DIR}/ilibav")
	#add_subdirectory("${INCLUDE_DIR}/iqsci/CMake" "${AUX_INCLUDE_DIR}/iqsci")
endif()
if(APPLE)
	# ---------- no longer used, but kept for reference ----------
#	add_subdirectory("${INCLUDE_DIR}/idc1394/CMake" "${AUX_INCLUDE_DIR}/idc1394")
endif()

# Impl

add_subdirectory("${IMPL_DIR}/IacfLoc/CMake" "${AUX_INCLUDE_DIR}/IacfLoc")
if(WIN32)
	add_subdirectory("${IMPL_DIR}/OpenCvPck/CMake" "${AUX_INCLUDE_DIR}/OpenCvPck")
	add_subdirectory("${IMPL_DIR}/QwtPck/CMake" "${AUX_INCLUDE_DIR}/QwtPck")
	add_subdirectory("${IMPL_DIR}/WinPck/CMake" "${AUX_INCLUDE_DIR}/WinPck")

	# ---------- no longer used, but kept for reference ----------
	#add_subdirectory("${IMPL_DIR}/FfMpegPck/CMake" "${AUX_INCLUDE_DIR}/FfMpegPck")
	#add_subdirectory("${IMPL_DIR}/QaxPck/CMake" "${AUX_INCLUDE_DIR}/QaxPck")
	#add_subdirectory("${IMPL_DIR}/QsciPck/CMake" "${AUX_INCLUDE_DIR}/QsciPck")
endif()
if(APPLE)
#	add_subdirectory("${IMPL_DIR}/Dc1394Pck/CMake" "${AUX_INCLUDE_DIR}/Dc1394Pck")
endif()

# Declare target-based deps so include paths/link order propagate transitively across IAcf libraries
# and onto the underlying Acf::/AcfSln:: libraries.
include("${IACFDIR}/Config/CMake/IAcfLibraryDependencies.cmake")

# Export IAcf libraries as a find_package(IAcf) package (build + install tree) so downstream
# modules (ImtCore) consume them via IAcf::<lib> targets instead of env vars/manual paths.
include("${IACFDIR}/Config/CMake/IAcfPackageExport.cmake")
