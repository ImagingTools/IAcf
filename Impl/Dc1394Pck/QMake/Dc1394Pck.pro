include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)

TARGET = Dc1394Pck

INCLUDEPATH += ../../../Include
INCLUDEPATH += $(ACFSLNDIR)/Include
INCLUDEPATH += $(LIBDC1394_2_2_0)

LIBS += -L../../../Lib/$$COMPILER_DIR -lidc1394

macx{
	LIBS += -L$(ACFSLNDIR)/Lib/$$COMPILER_DIR -licam
	LIBS += -L$(LIBDC1394_2_2_0)/lib -ldc1394
}
