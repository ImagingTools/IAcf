TARGET = iqwt3d

greaterThan(QT_MAJOR_VERSION, 5): QT += openglwidgets

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)
include($(IACFDIR)/Config/QMake/Qwt3d.pri)

INCLUDEPATH += ../..

FORMS += $$files($$_PRO_FILE_PWD_/../*.ui, false)

