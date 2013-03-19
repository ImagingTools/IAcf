include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/QtBaseConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)
include($(ACFDIR)/Config/QMake/AcfQt.pri)

TARGET = AbcPck

INCLUDEPATH += $(QTSERVICEDIR)/Src

LIBS += -L$(QTSERVICEDIR)/Lib/$$COMPILER_DIR -lQtService
LIBS += -L../../../Lib/$$COMPILER_DIR -liabc

