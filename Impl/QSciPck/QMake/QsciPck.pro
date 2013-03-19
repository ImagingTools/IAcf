include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/QtBaseConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)
include($(ACFDIR)/Config/QMake/AcfQt.pri)

TARGET = QsciPck

QT += xml

INCLUDEPATH += ../../../Include
INCLUDEPATH += "$(QSCINTILLA)/Include"

LIBS += -L../../../Lib/$$COMPILER_DIR -liqsci
LIBS += -L$(QSCINTILLA)/Lib/$$COMPILER_DIR -lQScintilla

