
INCLUDEPATH += $(OPENCVDIR_3_3_1)/include

LIBS += -L$(OPENCVDIR_3_3_1)/Lib/$$COMPILER_DIR

CONFIG(release, debug|release){
	LIBS += -lopencv_highgui331 -lopencv_core331 -lopencv_features2d331 -lopencv_objdetect331 -lopencv_imgproc331 -lopencv_videoio331 -lopencv_imgcodecs331
}

CONFIG(debug, debug|release){
	LIBS += -lopencv_highgui331d -lopencv_core331d -lopencv_features2d331d -lopencv_objdetect331d -lopencv_imgproc331d -lopencv_videoio331d -lopencv_imgcodecs331d
}

