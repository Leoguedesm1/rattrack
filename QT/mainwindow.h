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


    //Rattrack functions
    void le_video_file(VideoCapture& src, QString argv);
    void le_diretorio();
    void reset_video();
    void playorpause_video();
    void rattrack();
    void acha_perspectiva(Mat& transform_image, Mat& H2_warp, Size warpSize, int FHEIGHT, int FWIDTH);
    void aplica_perspectiva();
    void processa_video();
    void Threshold();
    void tracking(Mat aux, Mat track_image);
    Mat paint();
    void mostrar_imagem(Mat frame);
    void resetar_variaveis();
    void encerra_video();
    void salvar_imagens();
    Mat gera_IA();

    //Calibration functions
    void captureImagesCalibration(
        int* numSquares, int* numCornersHor,
        int* numCornersVer, int* numCorners,
        Size* board_sz, vector<Point2f> corners);

    double computeReprojectionErrors(
            const vector<vector<Point3f> >& objectPoints,
            const vector<vector<Point2f> >& imagePoints,
            const vector<Mat>& rvecs, const vector<Mat>& tvecs,
            const Mat& cameraMatrix, const Mat& distCoeffs,
            vector<float>& perViewErrors );

    void calcChessboardCorners(
        Size boardSize,
        float squareSize,
        vector<Point3f>& corners);

    bool runCalibration(
        vector<vector<Point2f> > imagePoints,
        Size imageSize, Size boardSize,
        float squareSize, float aspectRatio,
        int flags, Mat& cameraMatrix, Mat& distCoeffs,
        vector<Mat>& rvecs, vector<Mat>& tvecs,
        vector<float>& reprojErrs, double& totalAvgErr);

    void saveCameraParams(
        const string& filename, Size imageSize, Size boardSize,
        float squareSize, float aspectRatio, int flags,
        const Mat& cameraMatrix, const Mat& distCoeffs,
        const vector<Mat>& rvecs, const vector<Mat>& tvecs,
        const vector<float>& reprojErrs,
        const vector<vector<Point2f> >& imagePoints,
        double totalAvgErr );

    bool readStringList(
        const string& filename, vector<string>& l );

    bool runAndSave(
        const string& outputFilename,
        const vector<vector<Point2f> >& imagePoints,
        Size imageSize, Size boardSize, float squareSize,
        float aspectRatio, int flags, Mat& cameraMatrix,
        Mat& distCoeffs, bool writeExtrinsics, bool writePoints );

    bool getCalibrationCamera(Size* boardSize, vector<Point2f> corners, string getErro);

    bool getHomographyMatrix(Size *board_sz, int* numCornersHor, int* numCornersVer, int* numCorners, vector<Point2f> corners, string getErro);

    void setCalibrationCamera();

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

    vector<Point2f> coordinates;

};

#endif // MAINWINDOW_H
