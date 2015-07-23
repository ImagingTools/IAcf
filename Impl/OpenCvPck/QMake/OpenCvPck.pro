include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)

TARGET = OpenCvPck

INCLUDEPATH += $(ACFSLNDIR)/Include
INCLUDEPATH += $(OPENCVDIR_2_4_10)/Include

#LIBS += -L$(ACFDIR)/Lib/$$COMPILER_DIR -lAcfStd -lAcfQt
LIBS += -L$(ACFSLNDIR)/Lib/$$COMPILER_DIR -liblob -limeas -liipr
LIBS += -L../../../Lib/$$COMPILER_DIR -liocv
LIBS += -L$(OPENCVDIR_2_4_10)/lib/$$COMPILER_DIR -lopencv_imgproc2410 -lopencv_highgui2410 -lopencv_core2410

