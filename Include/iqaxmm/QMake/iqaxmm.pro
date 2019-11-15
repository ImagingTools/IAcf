TARGET = iqaxmm

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)

QT += multimedia axcontainer

INCLUDEPATH += $(ACFSLNDIR)/Include

HEADERS += ../Wrapped/*.h
SOURCES += ../Wrapped/*.cpp

