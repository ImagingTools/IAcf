include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtBaseConfig.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
include($(ACFCONFIGDIR)/QMake/AcfQt.pri)

TARGET = QwtPck

INCLUDEPATH += $(ACFSLNDIR)/Include
INCLUDEPATH += "$(QWTDIR_6_1_0)/Src"
INCLUDEPATH += "$(QWT3DDIR)/Include"

LIBS += -L../../../Lib/$$COMPILER_DIR -liqwt -liqwt3d
LIBS += -L$(QWTDIR_6_1_0)/Lib/$$COMPILER_DIR -lqwt
LIBS += -L$(QWT3DDIR)/Lib/$$COMPILER_DIR -lqwt3dlib

INCLUDEPATH += ../../../Include

