include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtBaseConfig.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
include($(ACFCONFIGDIR)/QMake/AcfQt.pri)

TARGET = QaxPck

QT += multimedia axcontainer

INCLUDEPATH += $(ACFSLNDIR)/Include

#LIBS += -L$(ACFSLNDIR)/Lib/$$COMPILER_DIR -limm
LIBS += -L../../../Lib/$$COMPILER_DIR -liqaxmm

CONFIG(debug, debug|release){
        LIBS += -lQt5AxContainerd -lQt5AxBased
}
CONFIG(release, debug|release){
        LIBS += -lQt5AxContainer -lQt5AxBase
}

