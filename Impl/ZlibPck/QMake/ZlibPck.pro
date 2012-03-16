include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)

TARGET = ZlibPck

INCLUDEPATH += ../../../Include
INCLUDEPATH += "$(ZLIBDIR)/include" 

LIBS += -L../../../Lib/$$COMPILER_DIR -lizlib
LIBS += -L$(ZLIBDIR)/lib -lzlib

