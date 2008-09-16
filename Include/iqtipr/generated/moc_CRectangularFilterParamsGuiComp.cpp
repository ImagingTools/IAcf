/****************************************************************************
** Meta object code from reading C++ file 'CRectangularFilterParamsGuiComp.h'
**
** Created: Tue 16. Sep 12:10:12 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CRectangularFilterParamsGuiComp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CRectangularFilterParamsGuiComp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_iqtipr__CRectangularFilterParamsGuiComp[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      47,   41,   40,   40, 0x09,
      86,   41,   40,   40, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_iqtipr__CRectangularFilterParamsGuiComp[] = {
    "iqtipr::CRectangularFilterParamsGuiComp\0"
    "\0value\0on_FilterWidthSlider_valueChanged(int)\0"
    "on_FilterHeightSlider_valueChanged(int)\0"
};

const QMetaObject iqtipr::CRectangularFilterParamsGuiComp::staticMetaObject = {
    { &iqt::TDesignerGuiObserverCompBase<Ui::CRectangularFilterParamsGuiComp,iipr::IMultidimensionalFilterParams>::staticMetaObject, qt_meta_stringdata_iqtipr__CRectangularFilterParamsGuiComp,
      qt_meta_data_iqtipr__CRectangularFilterParamsGuiComp, 0 }
};

const QMetaObject *iqtipr::CRectangularFilterParamsGuiComp::metaObject() const
{
    return &staticMetaObject;
}

void *iqtipr::CRectangularFilterParamsGuiComp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_iqtipr__CRectangularFilterParamsGuiComp))
	return static_cast<void*>(const_cast< CRectangularFilterParamsGuiComp*>(this));
    typedef iqt::TDesignerGuiObserverCompBase<Ui::CRectangularFilterParamsGuiComp,iipr::IMultidimensionalFilterParams> QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int iqtipr::CRectangularFilterParamsGuiComp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef iqt::TDesignerGuiObserverCompBase<Ui::CRectangularFilterParamsGuiComp,iipr::IMultidimensionalFilterParams> QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_FilterWidthSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: on_FilterHeightSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 2;
    }
    return _id;
}
