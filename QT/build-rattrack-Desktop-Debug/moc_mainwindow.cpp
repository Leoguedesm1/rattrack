/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../rattrack/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      39,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      32,   11,   11,   11, 0x08,
      51,   11,   11,   11, 0x08,
      74,   11,   11,   11, 0x08,
      95,   11,   11,   11, 0x08,
     115,   11,   11,   11, 0x08,
     135,   11,   11,   11, 0x08,
     155,   11,   11,   11, 0x08,
     178,  172,   11,   11, 0x08,
     208,  172,   11,   11, 0x08,
     235,  172,   11,   11, 0x08,
     262,  172,   11,   11, 0x08,
     301,  291,   11,   11, 0x08,
     321,   11,   11,   11, 0x08,
     339,   11,   11,   11, 0x08,
     356,   11,   11,   11, 0x08,
     381,   11,   11,   11, 0x08,
     401,   11,   11,   11, 0x08,
     430,  421,   11,   11, 0x08,
     467,   11,   11,   11, 0x08,
     482,   11,   11,   11, 0x08,
     496,   11,   11,   11, 0x08,
     516,   11,   11,   11, 0x08,
     527,   11,   11,   11, 0x08,
     548,   11,   11,   11, 0x08,
     565,   11,   11,   11, 0x08,
     593,  577,   11,   11, 0x08,
     615,   11,  611,   11, 0x08,
     629,  623,   11,   11, 0x08,
     649,   11,   11,   11, 0x08,
     669,   11,   11,   11, 0x08,
     685,   11,   11,   11, 0x08,
     698,   11,  611,   11, 0x08,
     708,   11,   11,   11, 0x08,
     728,   11,   11,   11, 0x08,
     804,  748,   11,   11, 0x08,
     916,  873,   11,   11, 0x08,
    1022,  950,   11,   11, 0x08,
    1159, 1121,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0on_btFile_clicked()\0"
    "on_btDir_clicked()\0on_btIniciar_clicked()\0"
    "on_btReset_clicked()\0on_btPlay_clicked()\0"
    "create_buttonTela()\0activePerspective()\0"
    "activeOriginal()\0value\0"
    "on_tbThresh_valueChanged(int)\0"
    "on_tbMin_valueChanged(int)\0"
    "on_tbMax_valueChanged(int)\0"
    "on_tbTrack_valueChanged(int)\0contFiles\0"
    "setFileName(double)\0reset_interface()\0"
    "inputNameTeste()\0on_btCamConfig_clicked()\0"
    "on_btSair_clicked()\0on_btSnap_clicked()\0"
    "src,argv\0le_video_file(VideoCapture&,QString)\0"
    "le_diretorio()\0reset_video()\0"
    "playorpause_video()\0rattrack()\0"
    "aplica_perspectiva()\0processa_video()\0"
    "Threshold()\0aux,track_image\0"
    "tracking(Mat,Mat)\0Mat\0paint()\0frame\0"
    "mostrar_imagem(Mat)\0resetar_variaveis()\0"
    "encerra_video()\0saveImages()\0gera_IA()\0"
    "calibrationCamera()\0assistCalibration()\0"
    "srcPoints,dstPoints,homography,center,radius,pixelRatio\0"
    "saveHomography(vector<Point2f>,vector<Point2f>,Mat,Point,int,double)\0"
    "imageSize,board_w,board_h,n_boards,measure\0"
    "saveInfos(Size,int,int,int,float)\0"
    "intrinsic_Matrix,distortion_coeffs,rvecs,tvecs,objectPoints,imagePoint"
    "s\0"
    "saveCalibration(Mat,Mat,vector<Mat>,vector<Mat>,vector<vector<Point3f>"
    " >,vector<vector<Point2f> >)\0"
    "before,atual,find,fileTest,frames,fps\0"
    "saveRatInfos(Point2d,Point2d,bool,ofstream&,int,double)\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_btFile_clicked(); break;
        case 1: _t->on_btDir_clicked(); break;
        case 2: _t->on_btIniciar_clicked(); break;
        case 3: _t->on_btReset_clicked(); break;
        case 4: _t->on_btPlay_clicked(); break;
        case 5: _t->create_buttonTela(); break;
        case 6: _t->activePerspective(); break;
        case 7: _t->activeOriginal(); break;
        case 8: _t->on_tbThresh_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_tbMin_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_tbMax_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->on_tbTrack_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->setFileName((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 13: _t->reset_interface(); break;
        case 14: _t->inputNameTeste(); break;
        case 15: _t->on_btCamConfig_clicked(); break;
        case 16: _t->on_btSair_clicked(); break;
        case 17: _t->on_btSnap_clicked(); break;
        case 18: _t->le_video_file((*reinterpret_cast< VideoCapture(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 19: _t->le_diretorio(); break;
        case 20: _t->reset_video(); break;
        case 21: _t->playorpause_video(); break;
        case 22: _t->rattrack(); break;
        case 23: _t->aplica_perspectiva(); break;
        case 24: _t->processa_video(); break;
        case 25: _t->Threshold(); break;
        case 26: _t->tracking((*reinterpret_cast< Mat(*)>(_a[1])),(*reinterpret_cast< Mat(*)>(_a[2]))); break;
        case 27: { Mat _r = _t->paint();
            if (_a[0]) *reinterpret_cast< Mat*>(_a[0]) = _r; }  break;
        case 28: _t->mostrar_imagem((*reinterpret_cast< Mat(*)>(_a[1]))); break;
        case 29: _t->resetar_variaveis(); break;
        case 30: _t->encerra_video(); break;
        case 31: _t->saveImages(); break;
        case 32: { Mat _r = _t->gera_IA();
            if (_a[0]) *reinterpret_cast< Mat*>(_a[0]) = _r; }  break;
        case 33: _t->calibrationCamera(); break;
        case 34: _t->assistCalibration(); break;
        case 35: _t->saveHomography((*reinterpret_cast< vector<Point2f>(*)>(_a[1])),(*reinterpret_cast< vector<Point2f>(*)>(_a[2])),(*reinterpret_cast< Mat(*)>(_a[3])),(*reinterpret_cast< Point(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6]))); break;
        case 36: _t->saveInfos((*reinterpret_cast< Size(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< float(*)>(_a[5]))); break;
        case 37: _t->saveCalibration((*reinterpret_cast< Mat(*)>(_a[1])),(*reinterpret_cast< Mat(*)>(_a[2])),(*reinterpret_cast< vector<Mat>(*)>(_a[3])),(*reinterpret_cast< vector<Mat>(*)>(_a[4])),(*reinterpret_cast< vector<vector<Point3f> >(*)>(_a[5])),(*reinterpret_cast< vector<vector<Point2f> >(*)>(_a[6]))); break;
        case 38: _t->saveRatInfos((*reinterpret_cast< Point2d(*)>(_a[1])),(*reinterpret_cast< Point2d(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< ofstream(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 39)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 39;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
