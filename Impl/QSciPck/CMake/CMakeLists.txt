project(QSciPck LANGUAGES CXX)


include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)

include_directories("$ENV{QSCINTILLA}/Include")
include_directories("$ENV{QSCINTILLA}/Qt4Qt5")

target_link_libraries(${PROJECT_NAME} ${ACF_PACKAGE_LINK_SCOPE}
	iqsci
)

target_link_directories(${PROJECT_NAME} PUBLIC $ENV{QSCINTILLA}/Lib/${TARGETNAME})
target_link_libraries(${PROJECT_NAME} ${ACF_PACKAGE_LINK_SCOPE}
QScintilla
)

