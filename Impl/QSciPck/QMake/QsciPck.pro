include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/QtBaseConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)
include($(ACFDIR)/Config/QMake/AcfQt.pri)

TARGET = QsciPck

QT += xml

INCLUDEPATH += ../../../Include
INCLUDEPATH += "$(QScintilla)/Include"

LIBS += -L../../../Lib/$$COMPILER_DIR -liqsci
LIBS += -L$(QScintilla)/Lib/$$COMPILER_DIR -lQScintilla

