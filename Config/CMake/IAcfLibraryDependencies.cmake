# ---------------------------------------------------------------------------
# Clean, target-based inter-library dependency graph for IAcf.
#
# This mirrors the approach introduced for the ACF foundation (Acf) in
# Config/CMake/AcfLibraryDependencies.cmake and for AcfSln in
# Config/CMake/AcfSlnLibraryDependencies.cmake: instead of relying on the final
# executable link to resolve symbols and on a hand-tuned build order, the
# dependencies between the IAcf libraries - and their dependencies onto the
# underlying Acf:: and AcfSln:: libraries - are declared here as target usage
# requirements. Include paths and link order then propagate transitively and
# automatically, both for the in-tree build and for downstream consumers that
# use find_package(IAcf) and link a single IAcf::<lib> target.
#
# The plain target_link_libraries() signature is used deliberately when
# ACF_LIBRARY_LINK_SCOPE is empty (matching the legacy IAcf/Acf CMake), and the
# keyword signature otherwise; CMake forbids mixing the plain and keyword
# signatures on the same target. For static libraries the dependency still
# propagates transitively to consumers regardless of signature.
#
# The dependencies below are derived from the #include graph of each library.
# Dependencies onto the underlying ACF / ACF-Solutions foundations are expressed
# through the Acf::<lib> / AcfSln::<lib> imported targets published by
# find_package(Acf) / find_package(AcfSln); entries whose target does not exist
# in the current configuration (for example when Acf/AcfSln are still consumed
# through the legacy environment-variable shim rather than find_package, or when
# a platform-specific library is not built) are silently ignored.
#
# Included once, centrally, from Build/CMake/CMakeLists.txt after all library
# targets have been created.
# ---------------------------------------------------------------------------

# --- Image processing / OpenCV ----------------------------------------------
declare_target_dependencies(iocv		Acf::iimg AcfSln::iblob AcfSln::imm AcfSln::iedge)

# --- Qwt plotting -----------------------------------------------------------
declare_target_dependencies(iqwt		Acf::iqtgui AcfSln::imeas Qt${QT_VERSION_MAJOR}::Svg)

declare_target_dependencies(iqwt3d	Acf::iqtgui)
if(QT_VERSION_MAJOR EQUAL 6)
	declare_target_dependencies(iqwt3d Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
endif()

# --- Windows integration ----------------------------------------------------
declare_target_dependencies(iwin		AcfSln::iinsp)

# --- CardBox ----------------------------------------------------------------
declare_target_dependencies(icbox		Acf::ifile)

# --- IEEE1394 / firewire cameras --------------------------------------------
declare_target_dependencies(idc1394		Acf::iimg AcfSln::icam)

# --- FFmpeg / libav ---------------------------------------------------------
declare_target_dependencies(ilibav		Acf::iimg AcfSln::icam AcfSln::iqtmm)

# --- Native / Windows helper ------------------------------------------------
declare_target_dependencies(inat		Acf::iqt iwin)

# --- Phonon multimedia ------------------------------------------------------
declare_target_dependencies(iphonon		Acf::iqtgui AcfSln::imm)

# --- ActiveX multimedia -----------------------------------------------------
declare_target_dependencies(iqaxmm		Acf::iqtgui AcfSln::iqtmm)

# --- QScintilla editor ------------------------------------------------------
declare_target_dependencies(iqsci		Acf::iqtgui)

# --- Arxc-generated static libraries ----------------------------------------
declare_target_dependencies(IacfLoc		AcfSln::AcfSlnLoc)
