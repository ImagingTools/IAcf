TARGET = iqwt3d

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

INCLUDEPATH += ../..
INCLUDEPATH += "$(QWT3DDIR)/Include"

FORMS += ../*.ui
UI_DIR = ../../GeneratedFiles/iqwt3d
