include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)

TARGET = MePck

INCLUDEPATH += ../../../Include
INCLUDEPATH += $(MEIDSDIR)/include

LIBS += -L$(MEIDSDIR)/lib -lmeIDSmain


