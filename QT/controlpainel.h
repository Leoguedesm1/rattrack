#ifndef CONTROLPAINEL_H
#define CONTROLPAINEL_H

#include <QWidget>
#include <QMainWindow>
#include <QTimer>
#include <QToolBar>
#include "dialogconfig.h"
#include "dialogsave.h"
#include "dialogfindcircle.h"
#include "mainwindow.h"

#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <limits>

#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include "opencv/cv.h"

using namespace cv;
using namespace std;

/*Estruturas Trackbar*/
/*Reset*/
typedef struct {
    VideoCapture* src;
}userdatareset;

extern VideoCapture src; //Video de entrada
extern userdatareset u;
extern string animal;
extern int teste;
extern QString argv;
extern Mat tela;
extern Mat out_original;
extern Mat out;
extern double raio;
extern Mat transform_mat;
extern QTimer *tmrTimer;
extern int cont;
extern Mat track;
extern Mat p;


namespace Ui {
class controlpainel;
}

class controlpainel : public QWidget
{
    Q_OBJECT

public:
    MainWindow *mainParent;
    explicit controlpainel(QWidget *parent = 0);
    ~controlpainel();
    Mat H2; //H2 = Homography, transform = perspectiva
    int FHEIGHT; //FHEIGHT = Altura do video
    int FWIDTH; //FWIDTH = Largura do video
    Size warpSize; //warpSize = tamanho do warp

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_btPlay_clicked();
    void on_btReset_clicked();
    void on_btSair_clicked();
    void on_btConfig_clicked();
    void on_btSave_clicked();
    void on_btCircle_clicked();

    void acha_perspectiva(Mat& transform, Mat& H2, Size warpSize, int FHEIGHT, int FWIDTH);
    void aplica_perspectiva();
    void processa_video();
    void Threshold( int, void* );
    void tracking(Mat aux, Mat track);
    static Mat paint();
    void rattrack();
    void mostra_tela(Mat& out_perspective1);

private:
    Ui::controlpainel *ui;
    dialogConfig *dc;
    DialogSave *ds;
    dialogFindCircle *dfc;
    QImage qimg;
 };

#endif // CONTROLPAINEL_H
