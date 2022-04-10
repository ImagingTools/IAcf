# Root of IACF project

TEMPLATE = subdirs
CONFIG += ordered


# Static libraries

win32{
	win32-msvc2013{
		SUBDIRS += iocv
		iocv.file = ../../Include/iocv/QMake/iocv.pro
	}

	!isEmpty(MSVC_VER){
		contains(MSVC_VER, "14.0") | contains(MSVC_VER, "15.0") | contains(MSVC_VER, "16.0"){
			SUBDIRS += iocv
			iocv.file = ../../Include/iocv/QMake/iocv.pro
		}
	}

lessThan(QT_MAJOR_VERSION, 6){
	SUBDIRS += iqaxmm
	iqaxmm.file = ../../Include/iqaxmm/QMake/iqaxmm.pro

	SUBDIRS += iqwt
	iqwt.file = ../../Include/iqwt/QMake/iqwt.pro

	SUBDIRS += iqwt3d
	iqwt3d.file = ../../Include/iqwt3d/QMake/iqwt3d.pro
}

	SUBDIRS += iwin
	iwin.file = ../../Include/iwin/QMake/iwin.pro

	!contains(QMAKE_HOST.arch, x86_64){
		SUBDIRS += ilibav
		ilibav.file = ../../Include/ilibav/QMake/ilibav.pro
	}

	win32-msvc2005 | win32-msvc2008{
#		SUBDIRS += icbox
#		icbox.file = ../../Include/icbox/QMake/icbox.pro

		SUBDIRS += iqsci
		iqsci.file = ../../Include/iqsci/QMake/iqsci.pro
	}
}

macx{
	SUBDIRS += idc1394
	idc1394.file = ../../Include/idc1394/QMake/idc1394.pro
}


# Translations

SUBDIRS += IacfLoc
IacfLoc.file = ../../Impl/IacfLoc/QMake/IacfLoc.pro


# Component packages

win32{
	win32-msvc2013{
		SUBDIRS += OpenCvPck
		OpenCvPck.file = ../../Impl/OpenCvPck/QMake/OpenCvPck.pro
	}
	
	!isEmpty(MSVC_VER){
		contains(MSVC_VER, "14.0") | contains(MSVC_VER, "15.0") | contains(MSVC_VER, "16.0"){
			SUBDIRS += OpenCvPck
			OpenCvPck.file = ../../Impl/OpenCvPck/QMake/OpenCvPck.pro
		}
	}

lessThan(QT_MAJOR_VERSION, 6){
	SUBDIRS += QwtPck
	QwtPck.file = ../../Impl/QwtPck/QMake/QwtPck.pro
}

	SUBDIRS += WinPck
	WinPck.file = ../../Impl/WinPck/QMake/WinPck.pro

	!contains(QMAKE_HOST.arch, x86_64){
		SUBDIRS += FfMpegPck
		FfMpegPck.file = ../../Impl/FfMpegPck/QMake/FfMpegPck.pro
	}

	win32-msvc2005 | win32-msvc2008{
	lessThan(QT_MAJOR_VERSION, 6){
		SUBDIRS += QaxPck
		QaxPck.file = ../../Impl/QaxPck/QMake/QaxPck.pro
	}

#		SUBDIRS += CboxPck
#		CboxPck.file = ../../Impl/CboxPck/QMake/CboxPck.pro

		SUBDIRS += QsciPck
		QsciPck.file = ../../Impl/QsciPck/QMake/QsciPck.pro
	}
}

macx{
	SUBDIRS += Dc1394Pck
	Dc1394Pck.file = ../../Impl/Dc1394Pck/QMake/Dc1394Pck.pro
}


