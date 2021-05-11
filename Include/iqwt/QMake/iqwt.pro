TARGET = iqwt

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)
include($(IACFDIR)/Config/QMake/Qwt.pri)

INCLUDEPATH += $(ACFSLNDIR)/Include

FORMS += $$files($$_PRO_FILE_PWD_/../*.ui, false)

