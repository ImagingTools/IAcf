TEMPLATE = subdirs

win32{
	SUBDIRS += AvtPck
	SUBDIRS += OpenCvPck
	SUBDIRS += MePck
	SUBDIRS += SmaGigEPck
	SUBDIRS += AbcPck
	SUBDIRS += CboxPck
	SUBDIRS += FfMpegPck
	SUBDIRS += QaxPck
	SUBDIRS += QsciPck
	SUBDIRS += QwtPck
	SUBDIRS += ZlibPck

	AvtPck.file = ../../Impl/AvtPck/QMake/AvtPck.pro
	OpenCvPck.file = ../../Impl/OpenCvPck/QMake/OpenCvPck.pro
	MePck.file = ../../Impl/MePck/QMake/MePck.pro
	SmaGigEPck.file = ../../Impl/SmaGigEPck/QMake/SmaGigEPck.pro
	AbcPck.file = ../../Impl/AbcPck/QMake/AbcPck.pro
	CboxPck.file = ../../Impl/CboxPck/QMake/CboxPck.pro
	FfMpegPck.file = ../../Impl/FfMpegPck/QMake/FfMpegPck.pro
	QaxPck.file = ../../Impl/QaxPck/QMake/QaxPck.pro
	QsciPck.file = ../../Impl/QsciPck/QMake/QsciPck.pro
	QwtPck.file = ../../Impl/QwtPck/QMake/QwtPck.pro
	ZlibPck.file = ../../Impl/ZlibPck/QMake/ZlibPck.pro
}

osx{
	SUBDIRS += Dc1394Pck

	Dc1394Pck.file = ../../Impl/Dc1394Pck/QMake/Dc1394Pck.pro
}
