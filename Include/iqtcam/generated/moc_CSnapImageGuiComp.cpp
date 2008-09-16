/****************************************************************************
** Meta object code from reading C++ file 'CSnapImageGuiComp.h'
**
** Created: Tue 16. Sep 12:10:09 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CSnapImageGuiComp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CSnapImageGuiComp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_iqtcam__CSnapImageGuiComp[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      27,   26,   26,   26, 0x09,
      64,   56,   26,   26, 0x09,
      97,   26,   26,   26, 0x09,
     126,   26,   26,   26, 0x09,
     156,   26,   26,   26, 0x09,
     186,   26,   26,   26, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_iqtcam__CSnapImageGuiComp[] = {
    "iqtcam::CSnapImageGuiComp\0\0"
    "on_SnapImageButton_clicked()\0checked\0"
    "on_LiveImageButton_toggled(bool)\0"
    "on_SaveImageButton_clicked()\0"
    "on_LoadParamsButton_clicked()\0"
    "on_SaveParamsButton_clicked()\0"
    "OnTimerReady()\0"
};

const QMetaObject iqtcam::CSnapImageGuiComp::staticMetaObject = {
    { &iqt2d::TSceneExtenderCompBase<iqt::TDesignerGuiCompBase<Ui::CSnapImageGuiComp,QWidget> >::staticMetaObject, qt_meta_stringdata_iqtcam__CSnapImageGuiComp,
      qt_meta_data_iqtcam__CSnapImageGuiComp, 0 }
};

const QMetaObject *iqtcam::CSnapImageGuiComp::metaObject() const
{
    return &staticMetaObject;
}

void *iqtcam::CSnapImageGuiComp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_iqtcam__CSnapImageGuiComp))
	return static_cast<void*>(const_cast< CSnapImageGuiComp*>(this));
    typedef iqt2d::TSceneExtenderCompBase<iqt::TDesignerGuiCompBase<Ui::CSnapImageGuiComp,QWidget> > QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int iqtcam::CSnapImageGuiComp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef iqt2d::TSceneExtenderCompBase<iqt::TDesignerGuiCompBase<Ui::CSnapImageGuiComp,QWidget> > QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_SnapImageButton_clicked(); break;
        case 1: on_LiveImageButton_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: on_SaveImageButton_clicked(); break;
        case 3: on_LoadParamsButton_clicked(); break;
        case 4: on_SaveParamsButton_clicked(); break;
        case 5: OnTimerReady(); break;
        }
        _id -= 6;
    }
    return _id;
}
