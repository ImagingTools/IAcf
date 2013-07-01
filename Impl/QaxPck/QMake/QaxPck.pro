include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtBaseConfig.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
include($(ACFCONFIGDIR)/QMake/AcfQt.pri)

TARGET = QaxPck

INCLUDEPATH += $(ACFSLNDIR)/Include

#LIBS += -L$(ACFSLNDIR)/Lib/$$COMPILER_DIR -limm
LIBS += -L../../../Lib/$$COMPILER_DIR -liqaxmm

CONFIG(debug, debug|release){
	LIBS += -lqaxcontainerd
}
CONFIG(release, debug|release){
	LIBS += -lqaxcontainer
}

