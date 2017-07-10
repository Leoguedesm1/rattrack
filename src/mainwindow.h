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
#include <QInputDialog>

//Bibliotecas C
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include <string.h>
#include <time.h>
#include <cctype>
#include <fstream>

//Bibliotecas OPENCV
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/opencv.hpp>
#include "opencv/cv.h"

using namespace std;
using namespace cv;

const string INFO_FILE_NAME = "/info.yml";
const string CALIB_FILE_NAME = "/calibration.yml";
const string MED_FILE_NAME = "/medidas.csv";
const string HOMOGRAPHY_FILE_NAME = "/homography.yml";
const string CALIBRATION_DIR_NAME = ((string) (QDir::currentPath()).toUtf8().constData()) + "/Calibration";
const string TESTES_DIR_NAME = ((string) (QDir::currentPath()).toUtf8().constData()) + "/Testes";

const double INF = std::numeric_limits<double>::infinity();

//Definindo valores de pixels BRANCO e PRETO
#define WHITE 255
#define BLACK 0

enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };

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
    void on_btDir_clicked();
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

    void setFileName(double contFiles);
    void reset_interface();
    void inputNameTeste();

    void on_btCamConfig_clicked();
    void on_btSair_clicked();

    void on_btSnap_clicked();


    //Rattrack functions
    void le_video_file(VideoCapture& src, QString argv);
    void le_diretorio();
    void reset_video();
    void playorpause_video();
    void rattrack();
    void aplica_perspectiva();
    void processa_video();
    void Threshold();
    void tracking(Mat aux, Mat track_image);
    Mat paint();
    void mostrar_imagem(Mat frame);
    void resetar_variaveis();
    void encerra_video();
    void saveImages();
    Mat gera_IA();
    //double calcMeters();
    //double calcVelo();
    //double calcAceleracao();

    //Calibration functions
    void calibrationCamera();
    void assistCalibration();
    void saveHomography(vector<Point2f> srcPoints, vector<Point2f> dstPoints, Mat homography, Point center, int radius, double pixelRatio);
    void saveInfos(Size imageSize, int board_w, int board_h, int n_boards, float measure);
    void saveCalibration(Mat intrinsic_Matrix, Mat distortion_coeffs, vector< Mat> rvecs, vector< Mat> tvecs,
                                     vector< vector <Point3f> > objectPoints, vector< vector <Point2f> > imagePoints);


    void saveRatInfos(Point2d before, Point2d atual, bool find, ofstream& fileTest, int frames, double fps);
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
    QStringList fileList;
    QString argv;
    QDir directory;

    int numberFiles;
    double contFiles;
    int snapshot;

    double executionTime;

    //Rattrack variables
    VideoCapture src;
    double fps;
    int frames;
    int total_frames;
    QTimer *tmrTimer;

    Mat src_frame, perspective_frame, original_frame, tela_image;
    Mat track_image, transform_image, p;
    Mat H2_warp;
    Size warpSize;
    int FHEIGHT, FWIDTH, centerX, centerY, threshold_value, min_area, max_area, rtrack;
    unsigned int cont_track;
    vector<Point2f> coordinates;
    double raio;
    double pixelRatio;

    ofstream fileTest;
    Point2d coordBefore, coordAtual;

};

#endif // MAINWINDOW_H
