project(QaxPck LANGUAGES CXX)


include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)

find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS AxContainer REQUIRED)

target_link_libraries(${PROJECT_NAME} ${ACF_PACKAGE_LINK_SCOPE}
	iqaxmm
	Qt${QT_VERSION_MAJOR}::AxContainer
)

