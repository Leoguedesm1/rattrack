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

using namespace cv;
using namespace std;

const string CALIBRATION_DIR_NAME = ((string) (QDir::currentPath()).toUtf8().constData()) + "/Calibration";
const string INFO_FILE_NAME = "/info.yml";
const string CALIB_FILE_NAME = "/calibration.yml";
const string HOMOGRAPHY_FILE_NAME = "/homography.yml";

class Calibration {
public:
    Calibration(QString fileName, int board_w, int board_h, int n_boards, float measure);
    void executeCalibration();
    bool getError();

private slots:


    void setCaptureVideo();

    Mat analyzisVideo();
    void writeImageInfos();
    void getCalibration();
    void writeCalibrationInfos(Mat intrinsic_Matrix, Mat distortion_coeffs, vector<Mat> rvecs, vector<Mat> tvecs);
    void getHomography(Mat imageTest);
    void writeHomographyInfos(vector<Point2f> srcPoints, vector<Point2f> dstPoints,  Mat homography2,
                              Point center, int radius, double pixelRatio);

private:
    int boardW, boardH;
    int nBoards;
    float measure;
    Size imageSize;
    QString fileName;
    VideoCapture captureVideo;

    vector< Point2f> corners;
    vector< vector< Point2f> > imagePoints;
    vector< vector< Point3f> > objectPoints;

    bool error;

};

#endif // CALIBRATION_H
