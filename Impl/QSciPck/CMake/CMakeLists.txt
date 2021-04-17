project(QSciPck LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/QtBaseConfig.cmake)
include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)

include_directories("$ENV{QSCINTILLA}/Include")
include_directories("$ENV{QSCINTILLA}/Qt4Qt5")

target_link_directories(${PROJECT_NAME} PUBLIC ../../../Lib/${TARGETNAME})
target_link_libraries(${PROJECT_NAME} -liqsci)
target_link_directories(${PROJECT_NAME} PUBLIC $ENV{QSCINTILLA}/Lib/${TARGETNAME})
target_link_libraries(${PROJECT_NAME} -lQScintilla)

