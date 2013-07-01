include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)

TARGET = WinPck

INCLUDEPATH += $(ACFSLNDIR)/Include

LIBS += -L../../../Lib/$$COMPILER_DIR -liwin

