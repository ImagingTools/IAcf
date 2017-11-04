INCLUDEPATH +=  $(OPENCVDIR_3_3_0)/include
LIBS += -L$(OPENCVDIR_3_3_0)/Lib/$$COMPILER_DIR

CONFIG(release, debug|release){
LIBS += -lopencv_highgui330 -lopencv_core330 -lopencv_features2d330 -lopencv_objdetect330 -lopencv_imgproc330
}

CONFIG(debug, debug|release){
	LIBS += -lopencv_highgui330d -lopencv_core330d -lopencv_features2d330d -lopencv_objdetect330d -lopencv_imgproc330d
}