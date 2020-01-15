
INCLUDEPATH += $(OPENCVDIR_3_3_1)/include

LIBS += -L$(OPENCVDIR_3_3_1)/Lib/$$COMPILER_DIR

unix{
	LIBS += -lopencv_highgui -lopencv_core -lopencv_features2d -lopencv_objdetect -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs -lopencv_calib3d -lopencv_photo331
}

win32{
	CONFIG(release, debug|release){
		LIBS += -lopencv_highgui331 -lopencv_core331 -lopencv_features2d331 -lopencv_objdetect331 -lopencv_imgproc331 -lopencv_videoio331 -lopencv_imgcodecs331 -lopencv_calib3d331 -lopencv_photo331
	}

	CONFIG(debug, debug|release){
		LIBS += -lopencv_highgui331d -lopencv_core331d -lopencv_features2d331d -lopencv_objdetect331d -lopencv_imgproc331d -lopencv_videoio331d -lopencv_imgcodecs331d -lopencv_calib3d331d -lopencv_photo331d
	}
}

