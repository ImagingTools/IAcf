cmake_minimum_required(VERSION 3.1.0)

project(IAcf)

if(NOT DEFINED ${ACFDIR})
	if(DEFINED ENV{ACFDIR})
		set(ACFDIR $ENV{ACFDIR})
	else()
		set(ACFDIR "${CMAKE_CURRENT_LIST_DIR}/../../../Acf")
	endif()
endif()

if(NOT DEFINED ${ACFSLNDIR})
	if(DEFINED ENV{ACFSLNDIR})
		set(ACFSLNDIR $ENV{ACFSLNDIR})
	else()
		set(ACFSLNDIR "${CMAKE_CURRENT_LIST_DIR}/../../../AcfSln")
	endif()
endif()

set(IACFDIR "${CMAKE_CURRENT_LIST_DIR}/../../../IAcf")

if(NOT DEFINED BUILDDIR)
	set(BUILDDIR "${CMAKE_CURRENT_LIST_DIR}/../../../")
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core)
find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS Core Widgets Core Gui Xml Network Svg Concurrent REQUIRED)

include(${ACFDIR}/Config/CMake/ProjectRoot.cmake)
include(${IACFDIR}/Config/CMake/IAcfEnv.cmake)


get_target_name(TARGETNAME)

include_directories("${ACFDIR}/Include")
include_directories("${ACFDIR}/Impl")
include_directories("${ACFDIR}/AuxInclude/${TARGETNAME}")
include_directories("${ACFSLNDIR}/AuxInclude/${TARGETNAME}")
link_directories(${ACFDIR}/Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME})
link_directories(${ACFSLNDIR}/Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME})


set(INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../Include")
set(IMPL_DIR "${CMAKE_CURRENT_LIST_DIR}/../../Impl")

set(AUX_INCLUDE_DIR "${BUILDDIR}/IAcf/AuxInclude/${TARGETNAME}/GeneratedFiles")
message("IACF AUX_INCLUDE_DIR " "${AUX_INCLUDE_DIR}")

include_directories("${BUILDDIR}/IAcf/AuxInclude/${TARGETNAME}")
include_directories("${BUILDDIR}/Acf/AuxInclude/${TARGETNAME}")

# set(AUX_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../AuxInclude/${TARGETNAME}/GeneratedFiles")
# message("IACF AUX_INCLUDE_DIR " "${AUX_INCLUDE_DIR}")

# include_directories("${CMAKE_CURRENT_LIST_DIR}/../../AuxInclude/${TARGETNAME}")
# include_directories("${CMAKE_CURRENT_LIST_DIR}/../../../Acf/AuxInclude/${TARGETNAME}")

include_directories("${ACFSLNDIR}/Include")
include_directories("${AUX_INCLUDE_DIR}/../")

# Include
if(WIN32)
	add_subdirectory("${INCLUDE_DIR}/iocv/CMake" "${AUX_INCLUDE_DIR}/iocv")
	#add_subdirectory("${INCLUDE_DIR}/iqaxmm/CMake" "${AUX_INCLUDE_DIR}/iqaxmm")
	add_subdirectory("${INCLUDE_DIR}/iqwt/CMake" "${AUX_INCLUDE_DIR}/iqwt")
	add_subdirectory("${INCLUDE_DIR}/iqwt3d/CMake" "${AUX_INCLUDE_DIR}/iqwt3d")
	add_subdirectory("${INCLUDE_DIR}/iwin/CMake" "${AUX_INCLUDE_DIR}/iwin")
	#add_subdirectory("${INCLUDE_DIR}/ilibav/CMake" "${AUX_INCLUDE_DIR}/ilibav")
	#add_subdirectory("${INCLUDE_DIR}/iqsci/CMake" "${AUX_INCLUDE_DIR}/iqsci")
endif()
if(APPLE)
#	add_subdirectory("${INCLUDE_DIR}/idc1394/CMake" "${AUX_INCLUDE_DIR}/idc1394")
endif()

# Impl

add_subdirectory("${IMPL_DIR}/IacfLoc/CMake" "${AUX_INCLUDE_DIR}/IacfLoc")
if(WIN32)
	add_subdirectory("${IMPL_DIR}/OpenCvPck/CMake" "${AUX_INCLUDE_DIR}/OpenCvPck")
	add_subdirectory("${IMPL_DIR}/QwtPck/CMake" "${AUX_INCLUDE_DIR}/QwtPck")
	add_subdirectory("${IMPL_DIR}/WinPck/CMake" "${AUX_INCLUDE_DIR}/WinPck")
	#add_subdirectory("${IMPL_DIR}/FfMpegPck/CMake" "${AUX_INCLUDE_DIR}/FfMpegPck")
	#add_subdirectory("${IMPL_DIR}/QaxPck/CMake" "${AUX_INCLUDE_DIR}/QaxPck")
	#add_subdirectory("${IMPL_DIR}/QsciPck/CMake" "${AUX_INCLUDE_DIR}/QsciPck")
endif()
if(APPLE)
#	add_subdirectory("${IMPL_DIR}/Dc1394Pck/CMake" "${AUX_INCLUDE_DIR}/Dc1394Pck")
endif()
