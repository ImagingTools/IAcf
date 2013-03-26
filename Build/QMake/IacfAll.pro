# Root of IACF project

include ($(ACFDIR)/Config/QMake/GeneralConfig.pri)

TEMPLATE = subdirs
CONFIG += ordered


#Static

win32{
	SUBDIRS += iocv
	iocv.file = ../../Include/iocv/QMake/iocv.pro

	SUBDIRS += ilibav
	ilibav.file = ../../Include/ilibav/QMake/ilibav.pro

	SUBDIRS += iqaxmm
	iqaxmm.file = ../../Include/iqaxmm/QMake/iqaxmm.pro

	SUBDIRS += iqwt
	iqwt.file = ../../Include/iqwt/QMake/iqwt.pro

	SUBDIRS += iqwt3d
	iqwt3d.file = ../../Include/iqwt3d/QMake/iqwt3d.pro

	SUBDIRS += izlib
	izlib.file = ../../Include/izlib/QMake/izlib.pro

	SUBDIRS += iwin
	iwin.file = ../../Include/iwin/QMake/iwin.pro


	win32-msvc2005 | win32-msvc2008{
		SUBDIRS += iabc
		iabc.file = ../../Include/iabc/QMake/iabc.pro

		SUBDIRS += icbox
		icbox.file = ../../Include/icbox/QMake/icbox.pro

		SUBDIRS += iqsci
		iqsci.file = ../../Include/iqsci/QMake/iqsci.pro
	}
}

macx{
	SUBDIRS += idc1394
	idc1394.file = ../../Include/idc1394/QMake/idc1394.pro
}


#Component

win32{
	SUBDIRS += OpenCvPck
	OpenCvPck.file = ../../Impl/OpenCvPck/QMake/OpenCvPck.pro

	SUBDIRS += MePck
	MePck.file = ../../Impl/MePck/QMake/MePck.pro

	SUBDIRS += FfMpegPck
	FfMpegPck.file = ../../Impl/FfMpegPck/QMake/FfMpegPck.pro

	SUBDIRS += QaxPck
	QaxPck.file = ../../Impl/QaxPck/QMake/QaxPck.pro

	SUBDIRS += QwtPck
	QwtPck.file = ../../Impl/QwtPck/QMake/QwtPck.pro

	SUBDIRS += ZlibPck
	ZlibPck.file = ../../Impl/ZlibPck/QMake/ZlibPck.pro

	SUBDIRS += WinPck
	WinPck.file = ../../Impl/WinPck/QMake/WinPck.pro

	win32-msvc2005 | win32-msvc2008{
		SUBDIRS += AbcPck
		AbcPck.file = ../../Impl/AbcPck/QMake/AbcPck.pro

		SUBDIRS += CboxPck
		CboxPck.file = ../../Impl/CboxPck/QMake/CboxPck.pro

		SUBDIRS += QsciPck
		QsciPck.file = ../../Impl/QsciPck/QMake/QsciPck.pro
	}
}

macx{
	SUBDIRS += Dc1394Pck
	Dc1394Pck.file = ../../Impl/Dc1394Pck/QMake/Dc1394Pck.pro
}
