include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

TARGET = iqwt

INCLUDEPATH += $(ACFSLNDIR)/Include
INCLUDEPATH += $(QWTDIR_6_1_0)/Src

FORMS += ../*.ui
UI_DIR = ../../GeneratedFiles/iqwt
