include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)

TARGET = WinPck

INCLUDEPATH += $(ACFSLNDIR)/Include

LIBS += -L../../../Lib/$$COMPILER_DIR -liwin

