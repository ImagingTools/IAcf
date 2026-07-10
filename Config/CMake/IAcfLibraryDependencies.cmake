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

# Declare the dependencies of an IAcf library, ignoring any entry whose target
# does not exist in the current configuration (for example platform-specific or
# feature-gated libraries, or Acf::/AcfSln:: targets that are not available
# because the legacy shim is used instead of find_package).
function(iacf_declare_library_dependencies target)
	if(NOT TARGET ${target})
		return()
	endif()

	foreach(dependency IN LISTS ARGN)
		if(TARGET ${dependency})
			target_link_libraries(${target} ${ACF_LIBRARY_LINK_SCOPE} ${dependency})
		endif()
	endforeach()
endfunction()

# --- Image processing / OpenCV ----------------------------------------------
iacf_declare_library_dependencies(iocv
	Acf::i2d Acf::ibase Acf::icomp Acf::ifile Acf::iimg Acf::ilog Acf::imod Acf::iprm Acf::istd
	AcfSln::iblob AcfSln::icalib AcfSln::icam AcfSln::iedge AcfSln::iinsp AcfSln::iipr AcfSln::imeas AcfSln::imm AcfSln::iproc)

# --- Qwt plotting -----------------------------------------------------------
iacf_declare_library_dependencies(iqwt		Acf::iqtgui Acf::imath Acf::imod Acf::iqt AcfSln::imeas)
iacf_declare_library_dependencies(iqwt3d	Acf::iqtgui Acf::imath Acf::iqt)

# --- Windows integration ----------------------------------------------------
iacf_declare_library_dependencies(iwin		Acf::icomp AcfSln::iinsp)

# --- CardBox ----------------------------------------------------------------
iacf_declare_library_dependencies(icbox		Acf::icomp Acf::ifile Acf::ilog Acf::iser)

# --- IEEE1394 / firewire cameras --------------------------------------------
iacf_declare_library_dependencies(idc1394	Acf::i2d Acf::icomp Acf::iimg Acf::ilog Acf::iprm
	AcfSln::icam AcfSln::imeas AcfSln::iproc)

# --- FFmpeg / libav ---------------------------------------------------------
iacf_declare_library_dependencies(ilibav	Acf::ifile Acf::iimg Acf::ilog
	AcfSln::icam AcfSln::imeas AcfSln::imm AcfSln::iqtmm)

# --- Native / Windows helper ------------------------------------------------
iacf_declare_library_dependencies(inat		Acf::iqt iwin)

# --- Phonon multimedia ------------------------------------------------------
iacf_declare_library_dependencies(iphonon	Acf::ibase Acf::iqt Acf::iqtgui AcfSln::imm)

# --- ActiveX multimedia -----------------------------------------------------
iacf_declare_library_dependencies(iqaxmm	Acf::iqtgui AcfSln::imm AcfSln::iqtmm)

# --- QScintilla editor ------------------------------------------------------
iacf_declare_library_dependencies(iqsci	Acf::ibase Acf::idoc Acf::imod Acf::iqtgui)

# --- Arxc-generated static libraries ----------------------------------------
iacf_declare_library_dependencies(IacfLoc	Acf::icomp)
