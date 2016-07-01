TARGET = iqwt

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

INCLUDEPATH += $(ACFSLNDIR)/Include
INCLUDEPATH += $(QWTDIR_6_1_2)/Src

FORMS += $$_PRO_FILE_PWD_/../*.ui
