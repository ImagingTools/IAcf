include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtBaseConfig.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
include($(ACFCONFIGDIR)/QMake/AcfQt.pri)

TARGET = QtMmPck

QT += svg phonon

LIBS += -L../../../Lib/$$COMPILER_DIR -liqtmm
