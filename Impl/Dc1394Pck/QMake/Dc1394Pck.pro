TARGET = Dc1394Pck

include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)

INCLUDEPATH += ../../../Include
INCLUDEPATH += $(ACFSLNDIR)/Include
INCLUDEPATH += $(LIBDC1394_2_2_0)

LIBS += -L../../../Lib/$$COMPILER_DIR -lidc1394

macx{
	LIBS += -L$(ACFSLNDIR)/Lib/$$COMPILER_DIR -licam
	LIBS += -L$(LIBDC1394_2_2_0)/lib -ldc1394
}

