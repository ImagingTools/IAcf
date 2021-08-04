TARGET = OpenCvPck

include(../../../../Acf/Config/QMake/ComponentConfig.pri)
include(../../../../Acf/Config/QMake/AcfStd.pri)
include(../../../Config/QMake/OpenCV_4_5_3.pri)

INCLUDEPATH += ../../../../AcfSln/Include

LIBS += -L../../../../AcfSln/Lib/$$COMPILER_DIR -liblob -limeas -liipr -licalib -liedge -liinsp
LIBS += -L../../../Lib/$$COMPILER_DIR -liocv


