include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

TARGET = iphonon

FORMS += ../*.ui
UI_DIR = ../../GeneratedFiles/iphonon

QT += phonon
