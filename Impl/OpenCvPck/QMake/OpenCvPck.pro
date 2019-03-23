include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
include($(IACFDIR)/Config/QMake/OpenCV_3_3_1.pri)

TARGET = OpenCvPck

INCLUDEPATH += $(ACFSLNDIR)/Include

LIBS += -L$(ACFSLNDIR)/Lib/$$COMPILER_DIR -liblob -limeas -liipr -licalib -liedge -liinsp
LIBS += -L../../../Lib/$$COMPILER_DIR -liocv


