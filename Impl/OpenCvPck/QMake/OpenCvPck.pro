include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)

TARGET = OpenCvPck

INCLUDEPATH += $(ACFSLNDIR)/Include
INCLUDEPATH += $(OPENCVDIR_2_4_10)/Include

LIBS += -L../../../Lib/$$COMPILER_DIR -liocv
LIBS += -L$(OPENCVDIR_2_4_10)/x86/$$COMPILER_NAME/lib -lopencv_highgui2410 -lopencv_core2410

