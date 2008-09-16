/****************************************************************************
** Meta object code from reading C++ file 'CCaliperParamsGuiComp.h'
**
** Created: Tue 16. Sep 12:10:12 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CCaliperParamsGuiComp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CCaliperParamsGuiComp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_iqtipr__CCaliperParamsGuiComp[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      37,   31,   30,   30, 0x09,
      76,   70,   30,   30, 0x09,
     119,   70,   30,   30, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_iqtipr__CCaliperParamsGuiComp[] = {
    "iqtipr::CCaliperParamsGuiComp\0\0value\0"
    "on_ThresholdSB_valueChanged(int)\0index\0"
    "on_EdgePolarityCB_currentIndexChanged(int)\0"
    "on_DirectionCB_currentIndexChanged(int)\0"
};

const QMetaObject iqtipr::CCaliperParamsGuiComp::staticMetaObject = {
    { &iqt::TDesignerGuiObserverCompBase<Ui::CCaliperParamsGuiComp,iipr::ICaliperParams>::staticMetaObject, qt_meta_stringdata_iqtipr__CCaliperParamsGuiComp,
      qt_meta_data_iqtipr__CCaliperParamsGuiComp, 0 }
};

const QMetaObject *iqtipr::CCaliperParamsGuiComp::metaObject() const
{
    return &staticMetaObject;
}

void *iqtipr::CCaliperParamsGuiComp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_iqtipr__CCaliperParamsGuiComp))
	return static_cast<void*>(const_cast< CCaliperParamsGuiComp*>(this));
    typedef iqt::TDesignerGuiObserverCompBase<Ui::CCaliperParamsGuiComp,iipr::ICaliperParams> QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int iqtipr::CCaliperParamsGuiComp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef iqt::TDesignerGuiObserverCompBase<Ui::CCaliperParamsGuiComp,iipr::ICaliperParams> QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_ThresholdSB_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: on_EdgePolarityCB_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: on_DirectionCB_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}
