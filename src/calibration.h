#ifndef CALIBRATION_H
#define CALIBRATION_H

//OpenCV Libraries
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

//C++ Libraries
#include<limits>

//Project Classes
#include "directorycreator.h"
#include "writerxml.h"
class CalibrationGUI;
#include "video.h"

using namespace cv;
using namespace std;

class Calibration
{
public:
    Calibration(Video* cap, int board_w, int board_h, int n_boards, float measure);
    void cancelCalibration();
    Point2d getCenter();
    void setCenter(Point2d point);
    void setRadius(double radius);
    double getRadius();
    void drawCircle();
    void drawLines();
    void addPoint();
    Point2d getPoint(int index);
    void setPoint(Point2d point, int index);
    void clearPoints();
    void removePoint(int index);
    void calcAngles();
    void writeCalibrationInfos();

private slots:
    bool findChessboardCalibration();
    void calcHomography();
    Point2d findIntersection(Point2d point);

private:
    CalibrationGUI *interface;
    Video* video;
    int boardW, boardH, nBoards;
    float measure;
    Size imageSize;
    double pixelRatio;

    vector< Point2f> corners;
    vector< vector< Point2f> > imagePoints;
    vector< vector< Point3f> > objectPoints;

    Mat homographyImage, homographyMatrix;
    vector< Point2f> srcPoints;
    vector< Point2f> dstPoints;

    DirectoryCreator *dirCreator;
    WriterInterface *writer;

    Point2d center;
    double radius;
    vector<Point2d> lines;
    vector<double> anglesQuad;
    Mat drawCircleMatrix;
};

#endif // CALIBRATION_H
