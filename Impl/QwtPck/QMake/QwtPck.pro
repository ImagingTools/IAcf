include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/QtBaseConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)
include($(ACFDIR)/Config/QMake/AcfQt.pri)

TARGET = QwtPck

INCLUDEPATH += $(ACFSLNDIR)/Include
INCLUDEPATH += "$(QWTDIR_6_0_0)/Include"
INCLUDEPATH += "$(QWT3DDIR)/Include"

LIBS += -L../../../Lib/$$COMPILER_DIR -liqwt -liqwt3d
#LIBS += -L$(ACFSLNDIR)/Lib/$$COMPILER_DIR -liqwt -liqwt3d
LIBS += -L$(QWTDIR_6_0_0)/Lib/$$COMPILER_DIR -lqwt
LIBS += -L$(QWT3DDIR)/Lib/$$COMPILER_DIR -lqwt3dlib

INCLUDEPATH += ../../../Include

