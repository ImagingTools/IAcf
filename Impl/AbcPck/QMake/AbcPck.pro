include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtBaseConfig.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
include($(ACFCONFIGDIR)/QMake/AcfQt.pri)

TARGET = AbcPck

INCLUDEPATH += $(QTSERVICEDIR)/Src

LIBS += -L$(QTSERVICEDIR)/Lib/$$COMPILER_DIR -lQtService
LIBS += -L../../../Lib/$$COMPILER_DIR -liabc

