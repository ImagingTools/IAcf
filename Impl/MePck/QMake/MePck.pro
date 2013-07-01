include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)

TARGET = MePck

INCLUDEPATH += $(ACFSLNDIR)/Include
INCLUDEPATH += $(MEIDSDIR)/include

LIBS += -L$(MEIDSDIR)/lib -lmeIDSmain


