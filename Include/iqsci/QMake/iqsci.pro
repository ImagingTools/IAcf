TARGET = iqsci

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

QT += xml

INCLUDEPATH += $(QSCINTILLA)/Include
INCLUDEPATH += $(QSCINTILLA)/Qt4Qt5

FORMS += $$files($$_PRO_FILE_PWD_/../*.ui, true)
UI_DIR = ../../GeneratedFiles/iqsci

