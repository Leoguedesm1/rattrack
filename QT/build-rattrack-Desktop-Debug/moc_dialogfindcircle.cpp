/****************************************************************************
** Meta object code from reading C++ file 'dialogfindcircle.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../rattrack/dialogfindcircle.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialogfindcircle.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_dialogFindCircle[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x0a,
      37,   17,   17,   17, 0x08,
      67,   61,   17,   17, 0x08,
      98,   61,   17,   17, 0x08,
     129,   17,   17,   17, 0x08,
     152,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_dialogFindCircle[] = {
    "dialogFindCircle\0\0encontra_circulo()\0"
    "on_btCancelar_clicked()\0value\0"
    "on_tbRaioMin_valueChanged(int)\0"
    "on_tbRaioMax_valueChanged(int)\0"
    "on_btDefault_clicked()\0on_btOK_clicked()\0"
};

void dialogFindCircle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        dialogFindCircle *_t = static_cast<dialogFindCircle *>(_o);
        switch (_id) {
        case 0: _t->encontra_circulo(); break;
        case 1: _t->on_btCancelar_clicked(); break;
        case 2: _t->on_tbRaioMin_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_tbRaioMax_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_btDefault_clicked(); break;
        case 5: _t->on_btOK_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData dialogFindCircle::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject dialogFindCircle::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_dialogFindCircle,
      qt_meta_data_dialogFindCircle, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &dialogFindCircle::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *dialogFindCircle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *dialogFindCircle::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_dialogFindCircle))
        return static_cast<void*>(const_cast< dialogFindCircle*>(this));
    return QWidget::qt_metacast(_clname);
}

int dialogFindCircle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE