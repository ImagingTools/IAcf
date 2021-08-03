
INCLUDEPATH += $(OPENCVDIR_4_5_3)/include

LIBS += -L$(OPENCVDIR_4_5_3)/lib/$$COMPILER_DIR

win32{
	CONFIG(release, debug|release){
		LIBS += -lopencv_world453
	}

	CONFIG(debug, debug|release){
		LIBS += -lopencv_world453d
	}
}

