/****************************************************************************
** Meta object code from reading C++ file 'CSnapBitmapSupplierGuiComp.h'
**
** Created: Tue 16. Sep 12:10:09 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CSnapBitmapSupplierGuiComp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CSnapBitmapSupplierGuiComp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_iqtcam__CSnapBitmapSupplierGuiComp[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      36,   35,   35,   35, 0x09,
      65,   35,   35,   35, 0x09,
      94,   35,   35,   35, 0x09,
     124,   35,   35,   35, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_iqtcam__CSnapBitmapSupplierGuiComp[] = {
    "iqtcam::CSnapBitmapSupplierGuiComp\0\0"
    "on_SnapImageButton_clicked()\0"
    "on_SaveImageButton_clicked()\0"
    "on_LoadParamsButton_clicked()\0"
    "on_SaveParamsButton_clicked()\0"
};

const QMetaObject iqtcam::CSnapBitmapSupplierGuiComp::staticMetaObject = {
    { &iqtproc::TSupplierGuiCompBase<Ui::CSnapBitmapSupplierGuiComp,icam::CSnapBitmapSupplierComp>::staticMetaObject, qt_meta_stringdata_iqtcam__CSnapBitmapSupplierGuiComp,
      qt_meta_data_iqtcam__CSnapBitmapSupplierGuiComp, 0 }
};

const QMetaObject *iqtcam::CSnapBitmapSupplierGuiComp::metaObject() const
{
    return &staticMetaObject;
}

void *iqtcam::CSnapBitmapSupplierGuiComp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_iqtcam__CSnapBitmapSupplierGuiComp))
	return static_cast<void*>(const_cast< CSnapBitmapSupplierGuiComp*>(this));
    typedef iqtproc::TSupplierGuiCompBase<Ui::CSnapBitmapSupplierGuiComp,icam::CSnapBitmapSupplierComp> QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int iqtcam::CSnapBitmapSupplierGuiComp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef iqtproc::TSupplierGuiCompBase<Ui::CSnapBitmapSupplierGuiComp,icam::CSnapBitmapSupplierComp> QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_SnapImageButton_clicked(); break;
        case 1: on_SaveImageButton_clicked(); break;
        case 2: on_LoadParamsButton_clicked(); break;
        case 3: on_SaveParamsButton_clicked(); break;
        }
        _id -= 4;
    }
    return _id;
}
