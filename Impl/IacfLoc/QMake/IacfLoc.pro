TARGET = AcfSlnLoc

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/CustomBuild.pri)

HEADERS =
SOURCES =
ACF_TRANSLATIONS += ../*.ts
ACF_TRANSLATIONS_OUTDIR = ../../GeneratedFiles/IacfLoc
RESOURCES += ../*.qrc

include($(ACFCONFIGDIR)/QMake/CustomBuild.pri)
