include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

TARGET = iqwt

INCLUDEPATH += $(ACFSLNDIR)/Include
INCLUDEPATH += "$(QPATH_QWT_0_6_1)/Src"

FORMS += ../*.ui
UI_DIR = ../../GeneratedFiles/iqwt
