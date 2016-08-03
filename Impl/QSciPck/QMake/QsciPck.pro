include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtBaseConfig.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
include($(ACFCONFIGDIR)/QMake/AcfQt.pri)

TARGET = QsciPck

QT += xml

INCLUDEPATH += ../../../Include
INCLUDEPATH += "$(QSCINTILLA)/Include"
INCLUDEPATH += $(QSCINTILLA)/Qt4Qt5

LIBS += -L../../../Lib/$$COMPILER_DIR -liqsci
LIBS += -L$(QSCINTILLA)/Lib/$$COMPILER_DIR -lQScintilla

