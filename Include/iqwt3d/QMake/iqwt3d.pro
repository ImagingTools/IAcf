TARGET = iqwt3d

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)
include($(IACFDIR)/Config/QMake/Qwt3d.pri)

INCLUDEPATH += ../..

FORMS += $$files($$_PRO_FILE_PWD_/../*.ui, true)

