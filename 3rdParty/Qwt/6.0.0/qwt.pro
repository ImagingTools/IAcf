TEMPLATE = lib
TARGET = qwt

COMPILER_NAME = QMake

win32-msvc*{
	COMPILER_NAME = VC

	win32-msvc2003{
		COMPILER_NAME = VC7
	}
	win32-msvc2005{
		COMPILER_NAME = VC8
	}
	win32-msvc2008{
		COMPILER_NAME = VC9
	}
	win32-msvc2010{
		COMPILER_NAME = VC10
	}
}

CONFIG(debug, debug|release){
	COMPILER_DIR = Debug$$COMPILER_NAME
}
CONFIG(release, debug|release){
	COMPILER_DIR = Release$$COMPILER_NAME
}

DESTDIR = ./Lib/$$COMPILER_DIR
CONFIG += staticlib

HEADERS += Include/*.h
SOURCES += Include/*.cpp
 
QT += gui core svg

