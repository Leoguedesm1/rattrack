#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//Bibliotecas QT
#include <QMainWindow>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QAction>
#include <QMenu>

//Bibliotecas C
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <limits>

//Bibliotecas OPENCV
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include "opencv/cv.h"

using namespace std;
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    //GUI functions
    void on_btFile_clicked();
    void on_btIniciar_clicked();
    void on_btReset_clicked();
    void on_btPlay_clicked();

    void create_buttonTela();
    void activePerspective();
    void activeOriginal();

    void on_tbThresh_valueChanged(int value);
    void on_tbMin_valueChanged(int value);
    void on_tbMax_valueChanged(int value);
    void on_tbTrack_valueChanged(int value);

    //Rattrack functions
    void le_video_file(VideoCapture& src, QString argv);
    void reset_video();
    void playorpause_video();
    void rattrack();
    void acha_perspectiva(Mat& transform_image, Mat& H2_warp, Size warpSize, int FHEIGHT, int FWIDTH);
    void aplica_perspectiva();
    void processa_video();
    void Threshold();
    void tracking(Mat aux, Mat track_image);
    Mat paint();
    void mostrar_imagem(Mat& frame);
    void resetar_variaveis();


private:

    //GUI variables
    Ui::MainWindow *ui;
    QLineEdit *animalInput;
    QLineEdit *testeInput;
    QAction *showPerspective;
    QAction *showOriginal;
    QMenu *tela_menu;
    int original, perspective;

    QString animal;
    QString teste;
    QString argv;

    //Rattrack variables
    VideoCapture src;
    double fps;
    int total_frames;
    QTimer *tmrTimer;

    Mat src_frame, perspective_frame, original_frame, tela_image;
    Mat track_image, transform_image, p;
    Mat H2_warp, invH_warp;

    Size warpSize;
    int FHEIGHT, FWIDTH, centerX, centerY, threshold_value, min_area, max_area, coordX, coordY, rtrack;
    unsigned int cont_track;

};

#endif // MAINWINDOW_H
