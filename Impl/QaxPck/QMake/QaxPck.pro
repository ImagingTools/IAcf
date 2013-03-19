include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/QtBaseConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)
include($(ACFDIR)/Config/QMake/AcfQt.pri)

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

