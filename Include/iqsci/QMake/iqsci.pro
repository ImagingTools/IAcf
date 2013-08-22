include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

TARGET = iqsci

QT += xml

INCLUDEPATH += $(QSCINTILLA)/Include

FORMS += ../*.ui
UI_DIR = ../../GeneratedFiles/iqsci
