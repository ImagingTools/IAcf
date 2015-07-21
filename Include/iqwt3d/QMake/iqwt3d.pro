TARGET = iqwt3d

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

INCLUDEPATH += ../..
INCLUDEPATH += "$(QWT3DDIR_0_3_1)/Include"

FORMS += ../*.ui
