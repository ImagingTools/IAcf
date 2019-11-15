TARGET = WinPck

include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)

INCLUDEPATH += $(ACFSLNDIR)/Include

LIBS += -L../../../Lib/$$COMPILER_DIR -liwin

