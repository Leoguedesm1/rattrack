/****************************************************************************
** Meta object code from reading C++ file 'dialogconfig.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../rattrack/dialogconfig.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialogconfig.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_dialogConfig[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      38,   32,   13,   13, 0x08,
      68,   32,   13,   13, 0x08,
      99,   32,   13,   13, 0x08,
     137,   32,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_dialogConfig[] = {
    "dialogConfig\0\0on_btOK_clicked()\0value\0"
    "on_tbThresh_valueChanged(int)\0"
    "on_tbMinArea_valueChanged(int)\0"
    "on_horizontalSlider_valueChanged(int)\0"
    "on_tbRaio_valueChanged(int)\0"
};

void dialogConfig::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        dialogConfig *_t = static_cast<dialogConfig *>(_o);
        switch (_id) {
        case 0: _t->on_btOK_clicked(); break;
        case 1: _t->on_tbThresh_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_tbMinArea_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_horizontalSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_tbRaio_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData dialogConfig::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject dialogConfig::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_dialogConfig,
      qt_meta_data_dialogConfig, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &dialogConfig::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *dialogConfig::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *dialogConfig::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_dialogConfig))
        return static_cast<void*>(const_cast< dialogConfig*>(this));
    return QWidget::qt_metacast(_clname);
}

int dialogConfig::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
