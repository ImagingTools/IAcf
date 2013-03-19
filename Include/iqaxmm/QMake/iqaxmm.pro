include($(ACFDIR)/Config/QMake/StaticConfig.pri)

TARGET = iqaxmm
QT += phonon

INCLUDEPATH += $(ACFSLNDIR)/Include

HEADERS += ../Wrapped/*.h
SOURCES += ../Wrapped/*.cpp

