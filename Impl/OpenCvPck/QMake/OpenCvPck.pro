include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)

TARGET = OpenCvPck

INCLUDEPATH += $(ACFSLNDIR)/Include
INCLUDEPATH += $(OPENCVDIR)/Include

LIBS += -L../../../Lib/$$COMPILER_DIR -liocv
LIBS += -L$(OPENCVDIR)/x86/$$COMPILER_NAME/lib -lopencv_highgui242

