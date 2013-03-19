include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)

TARGET = FfMpegPck

INCLUDEPATH += $(ACFSLNDIR)/Include
INCLUDEPATH += $(FFMPEGDIR_20121010)/include $(FFMPEGDIR_20121010)/msinttypes

LIBS += -L$(ACFSLNDIR)/Lib/$$COMPILER_DIR -licam -limeas
LIBS += -L../../../Lib/$$COMPILER_DIR -lilibav
LIBS += -L$(FFMPEGDIR_20121010)/lib -lswscale -lavdevice -lavformat -lavcodec -lavutil
