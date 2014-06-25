include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)

TARGET = iqaxmm
QT += multimedia axcontainer

INCLUDEPATH += $(ACFSLNDIR)/Include

HEADERS += ../Wrapped/*.h
SOURCES += ../Wrapped/*.cpp

