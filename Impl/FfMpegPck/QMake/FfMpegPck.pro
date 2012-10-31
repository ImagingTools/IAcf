include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)

TARGET = FfMpegPck

INCLUDEPATH += ../../../Include
INCLUDEPATH += $(FFMPEGDIR_20121010)/include $(FFMPEGDIR_20121010)/msinttypes

LIBS += -L../../../Lib/$$COMPILER_DIR -lilibav
LIBS += -L$(FFMPEGDIR_20121010)/lib -lswscale-0 -lavdevice-52 -lavformat-52 -lavcodec-52 -lavutil-49