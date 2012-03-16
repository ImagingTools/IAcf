include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)

TARGET = OpenCvPck

INCLUDEPATH += ../../../Include
INCLUDEPATH += $(OPENCVDIR)/Include

LIBS += -L../../../Lib/$$COMPILER_DIR -liocv
LIBS += -L$(OPENCVDIR)/Lib -llibhighgui200.dll.a

