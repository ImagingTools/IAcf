
INCLUDEPATH += $(OPENCVDIR_4_5_3)/include

OPENCV_LIBS = $$CONFIGURATION_NAME"_"$$COMPILER_CODE

LIBS += -L$(OPENCVDIR_4_5_3)/lib/$$OPENCV_LIBS

message("OpenCV Configuration")
message($$OPENCV_LIBS)

win32{
	CONFIG(release, debug|release){
		LIBS += -lopencv_world453
	}

	CONFIG(debug, debug|release){
		LIBS += -lopencv_world453d
	}
}

