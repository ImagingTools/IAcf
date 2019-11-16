TARGET = QwtPck

include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
include($(ACFCONFIGDIR)/QMake/AcfQt.pri)
include($(IACFDIR)/Config/QMake/Qwt.pri)
include($(IACFDIR)/Config/QMake/Qwt3d.pri)

QT += opengl

INCLUDEPATH += $(ACFSLNDIR)/Include

LIBS += -L../../../Lib/$$COMPILER_DIR -liqwt -liqwt3d
LIBS += -L$(ACFDIR)/Lib/$$COMPILER_DIR -liprm

win32{
	LIBS += -lopengl32 -lglu32
}

