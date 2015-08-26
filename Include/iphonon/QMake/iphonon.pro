include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

TARGET = iphonon

FORMS += $$_PRO_FILE_PWD_/../*.ui
UI_DIR = ../../GeneratedFiles/iphonon

QT += phonon
