include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)

TARGET = AvtPck

INCLUDEPATH += ../../../Include
INCLUDEPATH += $(AVTDIR)/FireGrab

LIBS += -L../../../Lib/$$COMPILER_DIR -liavt
LIBS += -L$(AVTDIR)/FireGrab -lFGCamera

