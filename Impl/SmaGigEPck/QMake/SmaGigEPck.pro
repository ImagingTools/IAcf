include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)

TARGET = SmaGigEPck

LIBS += -L../../../Lib/$$COMPILER_DIR
LIBS += -lisgige
LIBS += -L$(GIGEVISIONDIR_V2_5_4)/lib/$$PLATFORM_NAME -lGigEVisionSdk.lib

INCLUDEPATH += ../../../Include
INCLUDEPATH += "$(GIGEVISIONDIR_V2_5_4)/include" 


