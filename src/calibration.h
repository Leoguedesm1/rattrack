#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "opencv/cv.h"
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <QString>
#include <QDir>
#include<limits>
#include "outxml.h"

class CalibrationGUI;

using namespace cv;
using namespace std;

const string CALIBRATION_DIR_NAME = ((string) (QDir::currentPath()).toUtf8().constData()) + "/Calibration";
const string INFO_FILE_NAME = "/info.yml";
const string CALIB_FILE_NAME = "/calibration.yml";
const string HOMOGRAPHY_FILE_NAME = "/homography.yml";
const double INF = std::numeric_limits<double>::infinity();
const int L = 100;

class Calibration {

public:
    Calibration(QString fileName, int board_w, int board_h, int n_boards, float measure);
    void executeCalibration();
    void calcRadius();
    void writeHomographyInfos();
    void drawCircle(Point2d center, double radius);

private slots:

    void setCaptureVideo();
    VideoCapture getCaptureVideo();

    Mat analyzisVideo();
    void writeImageInfos();
    //void getCalibration();
    //void writeCalibrationInfos(Mat intrinsic_Matrix, Mat distortion_coeffs, vector<Mat> rvecs, vector<Mat> tvecs);
    void getHomography(Mat imageTest);
    Mat detectCircle();
    void getMouseCoordinate(int evt, int x, int y, int flags, void* ptr);

private:

    CalibrationGUI* cg;
    int boardW, boardH;
    int nBoards;
    float measure;
    Size imageSize;
    QString fileName;
    VideoCapture captureVideo;

    vector< Point2f> corners;
    vector< vector< Point2f> > imagePoints;
    vector< vector< Point3f> > objectPoints;

    double pixelRatio;
    Mat applyHomography;
    vector< Point2f> srcPoints;
    vector< Point2f> dstPoints;
    Mat homography2;
};

#endif // CALIBRATION_H
