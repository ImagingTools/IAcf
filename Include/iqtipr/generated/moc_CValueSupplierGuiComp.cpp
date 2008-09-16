/****************************************************************************
** Meta object code from reading C++ file 'CValueSupplierGuiComp.h'
**
** Created: Tue 16. Sep 12:10:12 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CValueSupplierGuiComp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CValueSupplierGuiComp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_iqtipr__CValueSupplierGuiComp[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      31,   30,   30,   30, 0x09,
      55,   30,   30,   30, 0x09,
      85,   30,   30,   30, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_iqtipr__CValueSupplierGuiComp[] = {
    "iqtipr::CValueSupplierGuiComp\0\0"
    "on_TestButton_clicked()\0"
    "on_LoadParamsButton_clicked()\0"
    "on_SaveParamsButton_clicked()\0"
};

const QMetaObject iqtipr::CValueSupplierGuiComp::staticMetaObject = {
    { &iqtproc::TSupplierGuiCompBase<Ui::CValueSupplierGuiComp,iproc::IValueSupplier>::staticMetaObject, qt_meta_stringdata_iqtipr__CValueSupplierGuiComp,
      qt_meta_data_iqtipr__CValueSupplierGuiComp, 0 }
};

const QMetaObject *iqtipr::CValueSupplierGuiComp::metaObject() const
{
    return &staticMetaObject;
}

void *iqtipr::CValueSupplierGuiComp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_iqtipr__CValueSupplierGuiComp))
	return static_cast<void*>(const_cast< CValueSupplierGuiComp*>(this));
    typedef iqtproc::TSupplierGuiCompBase<Ui::CValueSupplierGuiComp,iproc::IValueSupplier> QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int iqtipr::CValueSupplierGuiComp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef iqtproc::TSupplierGuiCompBase<Ui::CValueSupplierGuiComp,iproc::IValueSupplier> QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_TestButton_clicked(); break;
        case 1: on_LoadParamsButton_clicked(); break;
        case 2: on_SaveParamsButton_clicked(); break;
        }
        _id -= 3;
    }
    return _id;
}
