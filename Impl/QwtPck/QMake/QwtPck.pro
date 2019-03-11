TARGET = QwtPck


include(../../../../Acf/Config/QMake/ComponentConfig.pri)
include(../../../../Acf/Config/QMake/QtBaseConfig.pri)
include(../../../../Acf/Config/QMake/AcfStd.pri)
include(../../../../Acf/Config/QMake/AcfQt.pri)
include($(IACFDIR)/Config/QMake/Qwt.pri)
include($(IACFDIR)/Config/QMake/Qwt3d.pri)

INCLUDEPATH += ../../../Include
INCLUDEPATH += ../../../../AcfSln/Include

LIBS += -liqwt -liqwt3d

#Opengl
QT += opengl
LIBS += -L$(ACFDIR)/Lib/$$COMPILER_DIR -liprm

win32{
	LIBS += -lopengl32 -lglu32
}
