include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)

TARGET = ZlibPck

INCLUDEPATH += ../../../Include
INCLUDEPATH += "$(ZLIBDIR)/include" 

LIBS += -L../../../Lib/$$COMPILER_DIR -lizlib
LIBS += -L$(ZLIBDIR)/lib -lzlib

