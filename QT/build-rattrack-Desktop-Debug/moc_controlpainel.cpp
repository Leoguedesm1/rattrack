/****************************************************************************
** Meta object code from reading C++ file 'controlpainel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../rattrack/controlpainel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controlpainel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_controlpainel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      35,   14,   14,   14, 0x08,
      56,   14,   14,   14, 0x08,
      76,   14,   14,   14, 0x08,
     135,   98,   14,   14, 0x08,
     176,   14,   14,   14, 0x08,
     197,   14,   14,   14, 0x08,
     216,  214,   14,   14, 0x08,
     247,  237,   14,   14, 0x08,
     269,   14,  265,   14, 0x08,
     277,   14,   14,   14, 0x08,
     305,  288,   14,   14, 0x08,
     323,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_controlpainel[] = {
    "controlpainel\0\0on_btPlay_clicked()\0"
    "on_btReset_clicked()\0on_btSair_clicked()\0"
    "on_btConfig_clicked()\0"
    "transform,H2,warpSize,FHEIGHT,FWIDTH\0"
    "acha_perspectiva(Mat&,Mat&,Size,int,int)\0"
    "aplica_perspectiva()\0processa_video()\0"
    ",\0Threshold(int,void*)\0aux,track\0"
    "tracking(Mat,Mat)\0Mat\0paint()\0rattrack()\0"
    "out_perspective1\0mostra_tela(Mat&)\0"
    "on_btSave_clicked()\0"
};

void controlpainel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        controlpainel *_t = static_cast<controlpainel *>(_o);
        switch (_id) {
        case 0: _t->on_btPlay_clicked(); break;
        case 1: _t->on_btReset_clicked(); break;
        case 2: _t->on_btSair_clicked(); break;
        case 3: _t->on_btConfig_clicked(); break;
        case 4: _t->acha_perspectiva((*reinterpret_cast< Mat(*)>(_a[1])),(*reinterpret_cast< Mat(*)>(_a[2])),(*reinterpret_cast< Size(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 5: _t->aplica_perspectiva(); break;
        case 6: _t->processa_video(); break;
        case 7: _t->Threshold((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2]))); break;
        case 8: _t->tracking((*reinterpret_cast< Mat(*)>(_a[1])),(*reinterpret_cast< Mat(*)>(_a[2]))); break;
        case 9: { Mat _r = _t->paint();
            if (_a[0]) *reinterpret_cast< Mat*>(_a[0]) = _r; }  break;
        case 10: _t->rattrack(); break;
        case 11: _t->mostra_tela((*reinterpret_cast< Mat(*)>(_a[1]))); break;
        case 12: _t->on_btSave_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData controlpainel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject controlpainel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_controlpainel,
      qt_meta_data_controlpainel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &controlpainel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *controlpainel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *controlpainel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_controlpainel))
        return static_cast<void*>(const_cast< controlpainel*>(this));
    return QWidget::qt_metacast(_clname);
}

int controlpainel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
