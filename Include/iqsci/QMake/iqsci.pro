include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

TARGET = iqsci

QT += xml

INCLUDEPATH += $(QSCINTILLA)/Include
INCLUDEPATH += $(QSCINTILLA)/Qt4Qt5

FORMS += $$_PRO_FILE_PWD_/../*.ui
UI_DIR = ../../GeneratedFiles/iqsci
