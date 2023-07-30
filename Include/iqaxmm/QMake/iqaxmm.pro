TARGET = iqaxmm

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)

QT += multimedia axcontainer

INCLUDEPATH += $(ACFSLNDIR)/Include

HEADERS += ../Wrapped/axvlc.h
SOURCES += ../Wrapped/axvlc.cpp

