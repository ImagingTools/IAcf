TARGET = iphonon

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

FORMS += $$_PRO_FILE_PWD_/../*.ui
UI_DIR = ../../GeneratedFiles/iphonon

QT += phonon

