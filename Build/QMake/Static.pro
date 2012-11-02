TEMPLATE = subdirs

win32{
	SUBDIRS += iabc
	SUBDIRS += icbox
	SUBDIRS += ilibav
	SUBDIRS += iqaxmm
	SUBDIRS += iqsci
	SUBDIRS += iqwt
	SUBDIRS += iqwt3d
	SUBDIRS += izlib

	iabc.file = ../../Include/iabc/QMake/iabc.pro
	icbox.file = ../../Include/icbox/QMake/icbox.pro
	ilibav.file = ../../Include/ilibav/QMake/ilibav.pro
	iqaxmm.file = ../../Include/iqaxmm/QMake/iqaxmm.pro
	iqsci.file = ../../Include/iqsci/QMake/iqsci.pro
	iqwt.file = ../../Include/iqwt/QMake/iqwt.pro
	iqwt3d.file = ../../Include/iqwt3d/QMake/iqwt3d.pro
	izlib.file = ../../Include/izlib/QMake/izlib.pro
}

macx{
	SUBDIRS += idc1394

	idc1394.file = ../../Include/idc1394/QMake/idc1394.pro
}
